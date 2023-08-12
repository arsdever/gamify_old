#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVersionFunctionsFactory>
#include <fstream>

#include "renderer.hpp"

#include "common/logger.hpp"
#include "common/matrix.hpp"
#include "project/assets/material_asset.hpp"
#include "project/assets/mesh_asset.hpp"
#include "project/assets/shader_asset.hpp"
#include "project/assets/texture_asset.hpp"
#include "project/object.hpp"
#include "project/project.hpp"
#include "project/renderer_component.hpp"
#include "project/resource_manager.hpp"
#include "project/scene.hpp"
#include "project/transform_component.hpp"
#include <assimp/scene.h>

namespace g::rendering
{

renderer::~renderer() = default;

void renderer::resize(int width, int height)
{
    _width = width;
    _height = height;
}

void renderer::set_projection_matrix(common::matrix4x4f projection_matrix)
{
    _projection_matrix = std::move(projection_matrix);
}

void renderer::set_view_matrix(common::matrix4x4f view_matrix)
{
    _view_matrix = std::move(view_matrix);
}

namespace
{
common::logger_ptr logger = common::get_logger("rendering");
}

void renderer::setResourceManager(
    std::shared_ptr<project::resource_manager> resourceManager)
{
    project::resource_manager::init(resourceManager);
}

namespace impl
{

struct opengl_3_3_render_context : public project::render_context
{
    opengl_3_3_render_context() = default;
    ~opengl_3_3_render_context() override = default;

    unsigned program { 0 };
    unsigned vshader { 0 };
    unsigned fshader { 0 };

    unsigned vao { 0 };
    unsigned vbo { 0 };
    unsigned ebo { 0 };

    unsigned vertex_count { 0 };
    unsigned index_count { 0 };

    std::unordered_map<std::string, unsigned> uniforms;
    std::unordered_map<std::string, unsigned> attributes;
};

enum class shader_type
{
    vertex,
    fragment
};

class renderer : public ::g::rendering::renderer
{
public:
    renderer();
    ~renderer() override;

    void resize(int width, int height) override;
    void initialize() override;
    void render(std::shared_ptr<project::renderer_component> renderer) override;
    virtual void
    load_object(std::shared_ptr<project::renderer_component> renderer) override;

private:
    void initializeGLFunctions();
    unsigned create_shader(std::string_view source, shader_type type);
    unsigned create_program(std::vector<std::string_view> vertex_shaders,
                            std::vector<std::string_view> fragment_shaders);
    std::string load_shader_file(std::string_view path);

private:
    QOpenGLFunctions_3_3_Core* f;
    unsigned modelViewProjectionShader;
    unsigned lightingFragmentShader;
    unsigned default_vert;
    unsigned default_frag;
    unsigned gizmo_program { 0 };
    unsigned transform_vao { 0 };

    std::unordered_map<common::uuid, unsigned> _textures;
};

renderer::renderer() = default;

renderer::~renderer() = default;

void renderer::resize(int width, int height)
{
    initializeGLFunctions();
    f->glViewport(0, 0, width, height);
    ::g::rendering::renderer::resize(width, height);
}

void renderer::initialize()
{
    initializeGLFunctions();
    logger->debug("Initializing renderer");

    initializeGLFunctions();
    f->glClearColor(0.0f, 0.06f, 0.1f, 1.0f);

    modelViewProjectionShader =
        create_shader(load_shader_file("projection.vert"), shader_type::vertex);

    lightingFragmentShader =
        create_shader(load_shader_file("lighting.frag"), shader_type::fragment);

    default_vert =
        create_shader(load_shader_file("default.frag"), shader_type::vertex);
    default_frag =
        create_shader(load_shader_file("default.vert"), shader_type::fragment);

    gizmo_program = create_program({ load_shader_file("shaders/gizmo.vert") },
                                   { load_shader_file("shaders/gizmo.frag") });

    std::array<std::array<float, 6>, 6> transform_gizmo_points = {
        { { 1.0, 0.0, 0.0, 1.0, 0.0, 0.0 },
          { 0.0, 0.0, 0.0, 1.0, 0.0, 0.0 },
          { 0.0, 1.0, 0.0, 0.0, 1.0, 0.0 },
          { 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 },
          { 0.0, 0.0, 1.0, 0.0, 0.0, 1.0 },
          { 0.0, 0.0, 0.0, 0.0, 0.0, 1.0 } }
    };

    std::array<unsigned, 6> transform_gizmo_indices = { 0, 1, 2, 3, 4, 5 };

    f->glGenVertexArrays(1, &transform_vao);
    f->glBindVertexArray(transform_vao);
    unsigned transform_vbo, transform_ebo;
    f->glGenBuffers(1, &transform_vbo);
    f->glGenBuffers(1, &transform_ebo);
    f->glBindBuffer(GL_ARRAY_BUFFER, transform_vbo);
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, transform_ebo);
    f->glBufferData(GL_ARRAY_BUFFER,
                    transform_gizmo_points.size() * sizeof(float) * 6,
                    transform_gizmo_points.data(),
                    GL_STATIC_DRAW);
    f->glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                    sizeof(unsigned) * 6,
                    transform_gizmo_indices.data(),
                    GL_STATIC_DRAW);
    f->glVertexAttribPointer(0,
                             3,
                             GL_FLOAT,
                             GL_FALSE,
                             sizeof(float) * 6,
                             (void*)(sizeof(float) * 0));
    f->glVertexAttribPointer(1,
                             3,
                             GL_FLOAT,
                             GL_FALSE,
                             sizeof(float) * 6,
                             (void*)(sizeof(float) * 3));
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glBindVertexArray(0);
}

void renderer::render(std::shared_ptr<project::renderer_component> renderer)
{
    initializeGLFunctions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (renderer->enabled())
    {
        auto render_context =
            static_cast<opengl_3_3_render_context*>(renderer->render_context());
        if (!render_context)
        {
            logger->error("Render context is null");
            return;
        }

        f->glEnable(GL_DEPTH_TEST);
        f->glUseProgram(render_context->program);

        auto material = renderer->material();
        for (auto const& property : material->properties())
        {
            auto propertyLocation = f->glGetUniformLocation(
                render_context->program, property.first.c_str());
            if (propertyLocation == -1)
            {
                // logger->warn("Could not find uniform {}", property.first);
                continue;
            }

            if (std::holds_alternative<common::uuid>(property.second))
            {
                f->glActiveTexture(GL_TEXTURE0);
                f->glBindTexture(
                    GL_TEXTURE_2D,
                    _textures[ std::get<common::uuid>(property.second) ]);
            }
            else
            {
                f->glUniform3fv(propertyLocation,
                                1,
                                reinterpret_cast<const float*>(
                                    &std::get<common::color>(property.second)));
            }
        }

        f->glUniformMatrix4fv(render_context->uniforms[ "view" ],
                              1,
                              GL_FALSE,
                              _view_matrix.raw_data());
        f->glUniformMatrix4fv(render_context->uniforms[ "projection" ],
                              1,
                              GL_TRUE,
                              _projection_matrix.raw_data());

        common::matrix4x4f model_matrix = common::matrix4x4f::identity();
        f->glUniformMatrix4fv(render_context->uniforms[ "model" ],
                              1,
                              GL_FALSE,
                              model_matrix.raw_data());
        std::array<float, 3> position { _view_matrix(0, 3),
                                        _view_matrix(1, 3),
                                        _view_matrix(2, 3) };
        f->glUniform3fv(
            f->glGetUniformLocation(render_context->program, "view_position"),
            1,
            position.data());

        f->glBindVertexArray(render_context->vao);
        f->glDrawElements(
            GL_TRIANGLES, render_context->index_count, GL_UNSIGNED_INT, 0);

        f->glDisable(GL_DEPTH_TEST);
        f->glUseProgram(gizmo_program);
        f->glBindVertexArray(transform_vao);
        f->glUniformMatrix4fv(f->glGetUniformLocation(gizmo_program, "view"),
                              1,
                              GL_FALSE,
                              _view_matrix.raw_data());
        f->glUniformMatrix4fv(
            f->glGetUniformLocation(gizmo_program, "projection"),
            1,
            GL_TRUE,
            _projection_matrix.raw_data());
        model_matrix = renderer->transform()->matrix();
        f->glUniformMatrix4fv(f->glGetUniformLocation(gizmo_program, "model"),
                              1,
                              GL_FALSE,
                              model_matrix.raw_data());
        f->glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);
        f->glBindVertexArray(0);
        f->glUseProgram(0);
    }
}

void renderer::load_object(
    std::shared_ptr<project::renderer_component> renderer)
{
    initializeGLFunctions();
    auto* context = new opengl_3_3_render_context();

    auto mesh = renderer->mesh();

    if (mesh)
    {
        context->vertex_count = mesh->vertices().size();
        context->index_count = mesh->indices().size();

        f->glGenVertexArrays(1, &context->vao);
        f->glGenBuffers(1, &context->vbo);
        f->glGenBuffers(1, &context->ebo);

        f->glBindVertexArray(context->vao);
        f->glBindBuffer(GL_ARRAY_BUFFER, context->vbo);
        f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, context->ebo);

        f->glBufferData(GL_ARRAY_BUFFER,
                        mesh->vertices().size() *
                            sizeof(project::assets::mesh::vertex_t),
                        mesh->vertices().data(),
                        GL_STATIC_DRAW);
        f->glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                        mesh->indices().size() * sizeof(unsigned),
                        mesh->indices().data(),
                        GL_STATIC_DRAW);

        f->glVertexAttribPointer(0,
                                 3,
                                 GL_FLOAT,
                                 GL_FALSE,
                                 sizeof(float) * 8,
                                 (void*)(sizeof(float) * 0));
        f->glVertexAttribPointer(1,
                                 3,
                                 GL_FLOAT,
                                 GL_FALSE,
                                 sizeof(float) * 8,
                                 (void*)(sizeof(float) * 3));
        f->glVertexAttribPointer(2,
                                 2,
                                 GL_FLOAT,
                                 GL_FALSE,
                                 sizeof(float) * 8,
                                 (void*)(sizeof(float) * 6));
        f->glEnableVertexAttribArray(0);
        f->glEnableVertexAttribArray(1);
        f->glEnableVertexAttribArray(2);
        f->glBindVertexArray(0);
    }

    context->program = f->glCreateProgram();
    context->vshader = f->glCreateShader(GL_VERTEX_SHADER);
    context->fshader = f->glCreateShader(GL_FRAGMENT_SHADER);

    std::string error_log;
    int error_state;
    error_log.resize(512);

    auto material = renderer->material();
    if (material)
    {
        for (auto const& [ key, value ] : material->properties())
        {
            if (!key.starts_with("texture_"))
            {
                continue;
            }

            auto texture_uuid = std::get<common::uuid>(value);
            auto texture = std::static_pointer_cast<project::assets::texture>(
                project::resource_manager::get_resource_static<project::asset>(
                    texture_uuid));

            if (!texture)
            {
                logger->error("Could not find texture {}", texture_uuid);
                continue;
            }

            unsigned texture_id;
            f->glGenTextures(1, &texture_id);
            f->glBindTexture(GL_TEXTURE_2D, texture_id);
            f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            f->glTexImage2D(GL_TEXTURE_2D,
                            0,
                            GL_RGBA,
                            std::get<0>(texture->size()),
                            std::get<1>(texture->size()),
                            0,
                            GL_RGBA,
                            GL_UNSIGNED_BYTE,
                            texture->data().data());

            f->glGenerateMipmap(GL_TEXTURE_2D);
            _textures[ texture_uuid ] = texture_id;
        }

        auto shader = material->shader();

        std::string vshader_source = shader->vertex_shader_source();
        std::string fshader_source = shader->fragment_shader_source();

        const char* vsh = vshader_source.c_str();
        const char* fsh = fshader_source.c_str();

        f->glShaderSource(context->vshader, 1, &vsh, nullptr);
        f->glShaderSource(context->fshader, 1, &fsh, nullptr);

        f->glCompileShader(context->vshader);
        f->glGetShaderiv(context->vshader, GL_COMPILE_STATUS, &error_state);
        if (!error_state)
        {
            f->glGetShaderInfoLog(
                context->vshader, 512, nullptr, error_log.data());
            logger->error("Error compiling material's vertex shader: {}",
                          error_log);
        }

        f->glCompileShader(context->fshader);
        f->glGetShaderiv(context->fshader, GL_COMPILE_STATUS, &error_state);
        if (!error_state)
        {
            f->glGetShaderInfoLog(
                context->fshader, 512, nullptr, error_log.data());
            logger->error("Error compiling material's fragment shader: {}",
                          error_log);
        }

        f->glAttachShader(context->program, context->vshader);
        f->glAttachShader(context->program, context->fshader);
    }
    else
    {
        f->glAttachShader(context->program, default_vert);
        f->glAttachShader(context->program, default_frag);
    }

    f->glAttachShader(context->program, modelViewProjectionShader);
    f->glAttachShader(context->program, lightingFragmentShader);

    f->glLinkProgram(context->program);
    f->glGetProgramiv(context->program, GL_LINK_STATUS, &error_state);
    if (!error_state)
    {
        f->glGetProgramInfoLog(
            context->program, 512, nullptr, error_log.data());
        logger->error("Error linking material's shader program: {}", error_log);
    }

    context->uniforms[ "view" ] =
        f->glGetUniformLocation(context->program, "view");
    context->uniforms[ "projection" ] =
        f->glGetUniformLocation(context->program, "projection");
    context->uniforms[ "model" ] =
        f->glGetUniformLocation(context->program, "model");

    renderer->set_render_context(
        std::unique_ptr<project::render_context>(context));
}

void renderer::initializeGLFunctions()
{
    logger->debug("Trying to get OpenGL 3.3 functions");
    f = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_3_3_Core>(
        QOpenGLContext::currentContext());

    if (!f)
    {
        logger->error("OpenGL functions are not available");
        throw std::runtime_error("OpenGL functions are not available");
    }
}

unsigned renderer::create_shader(std::string_view source, shader_type type)
{
    unsigned shader = f->glCreateShader(
        type == shader_type::vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);

    auto* sourceC = source.data();
    f->glShaderSource(shader, 1, &sourceC, nullptr);
    f->glCompileShader(shader);
    int success;
    f->glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[ 512 ];
        f->glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        throw std::runtime_error("Error compiling shader");
    }

    return shader;
}

unsigned
renderer::create_program(std::vector<std::string_view> vertex_shaders,
                         std::vector<std::string_view> fragment_shaders)
{
    unsigned program = f->glCreateProgram();
    std::vector<unsigned> shaders;
    for (auto const& vertex_shader : vertex_shaders)
    {
        shaders.push_back(create_shader(vertex_shader, shader_type::vertex));
        f->glAttachShader(program, shaders.back());
    }

    for (auto const& fragment_shader : fragment_shaders)
    {
        shaders.push_back(
            create_shader(fragment_shader, shader_type::fragment));
        f->glAttachShader(program, shaders.back());
    }

    f->glLinkProgram(program);
    int success;
    f->glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[ 512 ];
        f->glGetProgramInfoLog(program, 512, nullptr, infoLog);
        throw std::runtime_error("Error linking program");
    }

    for (auto shader : shaders)
    {
        f->glDeleteShader(shader);
    }

    return program;
}

std::string renderer::load_shader_file(std::string_view path)
{
    std::ifstream shader_file(path.data());
    std::string content((std::istreambuf_iterator<char>(shader_file)),
                        std::istreambuf_iterator<char>());
    return content;
}

} // namespace impl
} // namespace g::rendering

extern "C"
{
    g::rendering::renderer* create_renderer()
    {
        static g::rendering::impl::renderer* renderer =
            new g::rendering::impl::renderer {};
        g::rendering::logger->debug("Creating renderer");
        return renderer;
    }

    void destroy_renderer(g::rendering::renderer* renderer)
    {
        g::rendering::logger->debug("Destroying renderer");
        // return new g::rendering::impl::renderer {};
    }
}
