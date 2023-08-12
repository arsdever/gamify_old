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
    f->glEnable(GL_DEPTH_TEST);

    std::string error_log;
    int error_state;
    error_log.resize(512);

    std::ifstream modelViewProjectionShaderFile("projection.vert");
    std::string modelViewProjectionShaderSource(
        (std::istreambuf_iterator<char>(modelViewProjectionShaderFile)),
        std::istreambuf_iterator<char>());
    auto* modelViewProjectionShaderSourceC =
        modelViewProjectionShaderSource.c_str();

    std::ifstream lightingFragmentShaderFile("lighting.frag");
    std::string lightingFragmentShaderSource(
        (std::istreambuf_iterator<char>(lightingFragmentShaderFile)),
        std::istreambuf_iterator<char>());
    auto* lightingFragmentShaderSourceC = lightingFragmentShaderSource.c_str();

    modelViewProjectionShader = f->glCreateShader(GL_VERTEX_SHADER);
    f->glShaderSource(modelViewProjectionShader,
                      1,
                      &modelViewProjectionShaderSourceC,
                      nullptr);
    f->glCompileShader(modelViewProjectionShader);
    f->glGetShaderiv(
        modelViewProjectionShader, GL_COMPILE_STATUS, &error_state);
    if (!error_state)
    {
        f->glGetShaderInfoLog(
            modelViewProjectionShader, 512, nullptr, error_log.data());
        logger->error("Error compiling modelViewProjectionShader: {}",
                      error_log);
    }

    lightingFragmentShader = f->glCreateShader(GL_FRAGMENT_SHADER);
    f->glShaderSource(
        lightingFragmentShader, 1, &lightingFragmentShaderSourceC, nullptr);
    f->glCompileShader(lightingFragmentShader);
    f->glGetShaderiv(lightingFragmentShader, GL_COMPILE_STATUS, &error_state);
    if (!error_state)
    {
        f->glGetShaderInfoLog(
            lightingFragmentShader, 512, nullptr, error_log.data());
        logger->error("Error compiling lighting fragment shader: {}",
                      error_log);
    }

    default_vert = f->glCreateShader(GL_VERTEX_SHADER);
    default_frag = f->glCreateShader(GL_FRAGMENT_SHADER);

    std::ifstream default_vertex_shader_file("default.vert");
    std::string default_vertex_shader_source(
        (std::istreambuf_iterator<char>(default_vertex_shader_file)),
        std::istreambuf_iterator<char>());
    std::ifstream default_fragment_shader_file("default.frag");
    std::string default_fragment_shader_source(
        (std::istreambuf_iterator<char>(default_fragment_shader_file)),
        std::istreambuf_iterator<char>());

    auto* default_vertex_shader_sourceC = default_vertex_shader_source.c_str();
    auto* default_fragment_shader_sourceC =
        default_fragment_shader_source.c_str();

    f->glShaderSource(default_vert, 1, &default_vertex_shader_sourceC, nullptr);
    f->glCompileShader(default_vert);
    f->glGetShaderiv(default_vert, GL_COMPILE_STATUS, &error_state);
    if (!error_state)
    {
        f->glGetShaderInfoLog(default_vert, 512, nullptr, error_log.data());
        logger->error("Error compiling default vertex shader: {}", error_log);
    }

    f->glShaderSource(
        default_frag, 1, &default_fragment_shader_sourceC, nullptr);
    f->glCompileShader(default_frag);
    f->glGetShaderiv(default_frag, GL_COMPILE_STATUS, &error_state);
    if (!error_state)
    {
        f->glGetShaderInfoLog(default_frag, 512, nullptr, error_log.data());
        logger->error("Error compiling default fragment shader: {}", error_log);
    }

    gizmo_program = f->glCreateProgram();
    std::ifstream gizmo_vertex_shader_file("shaders/gizmo.vert");
    std::ifstream gizmo_fragment_shader_file("shaders/gizmo.frag");
    std::string gizmo_vertex_shader_source(
        (std::istreambuf_iterator<char>(gizmo_vertex_shader_file)),
        std::istreambuf_iterator<char>());
    std::string gizmo_fragment_shader_source(
        (std::istreambuf_iterator<char>(gizmo_fragment_shader_file)),
        std::istreambuf_iterator<char>());
    auto* gizmo_vertex_shader_sourceC = gizmo_vertex_shader_source.c_str();
    auto* gizmo_fragment_shader_sourceC = gizmo_fragment_shader_source.c_str();

    unsigned gizmo_vertex_shader = f->glCreateShader(GL_VERTEX_SHADER);
    unsigned gizmo_fragment_shader = f->glCreateShader(GL_FRAGMENT_SHADER);

    f->glShaderSource(
        gizmo_vertex_shader, 1, &gizmo_vertex_shader_sourceC, nullptr);
    f->glCompileShader(gizmo_vertex_shader);
    f->glGetShaderiv(gizmo_vertex_shader, GL_COMPILE_STATUS, &error_state);
    if (!error_state)
    {
        f->glGetShaderInfoLog(
            gizmo_vertex_shader, 512, nullptr, error_log.data());
        logger->error("Error compiling gizmo vertex shader: {}", error_log);
    }

    f->glShaderSource(
        gizmo_fragment_shader, 1, &gizmo_fragment_shader_sourceC, nullptr);
    f->glCompileShader(gizmo_fragment_shader);
    f->glGetShaderiv(gizmo_fragment_shader, GL_COMPILE_STATUS, &error_state);
    if (!error_state)
    {
        f->glGetShaderInfoLog(
            gizmo_fragment_shader, 512, nullptr, error_log.data());
        logger->error("Error compiling gizmo fragment shader: {}", error_log);
    }

    f->glAttachShader(gizmo_program, gizmo_vertex_shader);
    f->glAttachShader(gizmo_program, gizmo_fragment_shader);
    f->glLinkProgram(gizmo_program);
    f->glGetProgramiv(gizmo_program, GL_LINK_STATUS, &error_state);
    if (!error_state)
    {
        f->glGetProgramInfoLog(gizmo_program, 512, nullptr, error_log.data());
        logger->error("Error linking gizmo shader program: {}", error_log);
    }

    f->glDeleteShader(gizmo_vertex_shader);
    f->glDeleteShader(gizmo_fragment_shader);

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
