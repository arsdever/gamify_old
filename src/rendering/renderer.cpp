#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVersionFunctionsFactory>
#include <fstream>

#include "rendering/renderer.hpp"

#include "common/logger.hpp"
#include "common/matrix.hpp"
#include "project/assets/material_asset.hpp"
#include "project/assets/mesh_asset.hpp"
#include "project/assets/shader_asset.hpp"
#include "project/object.hpp"
#include "project/project.hpp"
#include "project/renderer_component.hpp"
#include "project/resource_manager.hpp"
#include "project/scene.hpp"
#include <assimp/scene.h>

namespace g::rendering
{

renderer::~renderer() = default;

void renderer::set_projection_matrix(common::matrix4x4 projection_matrix)
{
    _projection_matrix = std::move(projection_matrix);
}

void renderer::set_view_matrix(common::matrix4x4 view_matrix)
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

    void initialize() override;
    void render(std::shared_ptr<project::renderer_component> renderer) override;
    virtual void
    load_object(std::shared_ptr<project::renderer_component> renderer) override;

    void draw_cube(common::vector3 position,
                   common::vector3 size,
                   common::vector3 color) override;

private:
    unsigned load_shader_from_file(std::string_view path);
    unsigned load_shader_from_string(std::string_view source, int type);

    QOpenGLFunctions_3_3_Core* checkAndGetGLFunctions();

    unsigned modelViewProjectionShader;
    unsigned lightingFragmentShader;
    unsigned default_vert;
    unsigned default_frag;
    unsigned gizmos_vert;
    unsigned gizmos_frag;
    unsigned _gizmos_program;
};

renderer::renderer() = default;

renderer::~renderer() = default;

void renderer::initialize()
{
    logger->debug("Initializing renderer");

    auto* f = checkAndGetGLFunctions();

    f->glEnable(GL_DEPTH_TEST);

    modelViewProjectionShader = load_shader_from_file("projection.vert");
    lightingFragmentShader = load_shader_from_file("lighting.frag");
    default_vert = load_shader_from_file("default.vert");
    default_frag = load_shader_from_file("default.frag");
    gizmos_vert = load_shader_from_file("gizmos.vert");
    gizmos_frag = load_shader_from_file("gizmos.frag");

    _gizmos_program = f->glCreateProgram();
    f->glAttachShader(_gizmos_program, modelViewProjectionShader);
    f->glAttachShader(_gizmos_program, gizmos_vert);
    f->glAttachShader(_gizmos_program, gizmos_frag);
    f->glLinkProgram(_gizmos_program);
    int success;
    f->glGetProgramiv(_gizmos_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[ 512 ];
        f->glGetProgramInfoLog(_gizmos_program, 512, nullptr, infoLog);
        logger->error("Shader program linking failed: {}", infoLog);
    }
}

void renderer::render(std::shared_ptr<project::renderer_component> renderer)
{
    auto* f = checkAndGetGLFunctions();
    if (renderer->enabled())
    {
        auto render_context =
            static_cast<opengl_3_3_render_context*>(renderer->render_context());
        if (!render_context)
        {
            logger->error("Render context is null");
            return;
        }

        f->glUseProgram(render_context->program);

        auto material = renderer->material();
        for (auto const& property : material->properties())
        {
            auto propertyLocation = f->glGetUniformLocation(
                render_context->program, property.first.c_str());
            if (propertyLocation == -1)
            {
                logger->warn("Could not find uniform {}. May be because of shader compiler optimizations.", property.first);
                continue;
            }

            f->glUniform3fv(propertyLocation,
                            1,
                            reinterpret_cast<const float*>(
                                &std::get<common::color>(property.second)));
        }

        f->glUniformMatrix4fv(render_context->uniforms[ "view" ],
                              1,
                              GL_FALSE,
                              _view_matrix.data.data());
        f->glUniformMatrix4fv(render_context->uniforms[ "projection" ],
                              1,
                              GL_FALSE,
                              _projection_matrix.data.data());

        QMatrix4x4 model_matrix;
        model_matrix.setToIdentity();
        f->glUniformMatrix4fv(render_context->uniforms[ "model" ],
                              1,
                              GL_FALSE,
                              model_matrix.data());
        std::array<float, 3> position = { _view_matrix.data[ 3 ],
                                          _view_matrix.data[ 7 ],
                                          _view_matrix.data[ 11 ] };
        f->glUniform3fv(
            f->glGetUniformLocation(render_context->program, "view_position"),
            1,
            position.data());

        f->glBindVertexArray(render_context->vao);
        f->glDrawElements(
            GL_TRIANGLES, render_context->index_count, GL_UNSIGNED_INT, 0);
        f->glBindVertexArray(0);
        f->glUseProgram(0);
    }
}

void renderer::draw_cube(common::vector3 position,
                         common::vector3 size,
                         common::vector3 color)
{
    auto* f = checkAndGetGLFunctions();
    // generate cube with 1 side length
    std::array<float, 24> cube_vertices = {
        -0.5, -0.5, -0.5, // left bottom back
        0.5,  -0.5, -0.5, // right bottom back
        0.5,  0.5,  -0.5, // right top back
        -0.5, 0.5,  -0.5, // left top back
        -0.5, -0.5, 0.5,  // left bottom front
        0.5,  -0.5, 0.5,  // right bottom front
        0.5,  0.5,  0.5,  // right top front
        -0.5, 0.5,  0.5   // left top front
    };

    // generate indices for cube sides
    std::array<unsigned, 36> cube_indices = {
        0, 1, 2, 2, 3, 0, // back
        1, 5, 6, 6, 2, 1, // right
        7, 6, 5, 5, 4, 7, // front
        4, 0, 3, 3, 7, 4, // left
        4, 5, 1, 1, 0, 4, // bottom
        3, 2, 6, 6, 7, 3  // top
    };

    QMatrix4x4 model_matrix;
    model_matrix.setToIdentity();
    model_matrix.translate(position.x, position.y, position.z);
    model_matrix.scale(size.x, size.y, size.z);

    unsigned vao, vbo, ebo;
    f->glGenVertexArrays(1, &vao);
    f->glGenBuffers(1, &vbo);
    f->glGenBuffers(1, &ebo);
    f->glBindVertexArray(vao);
    f->glBindBuffer(GL_ARRAY_BUFFER, vbo);
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    f->glBufferData(GL_ARRAY_BUFFER,
                    cube_vertices.size() * sizeof(float),
                    cube_vertices.data(),
                    GL_STATIC_DRAW);
    f->glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                    cube_indices.size() * sizeof(unsigned),
                    cube_indices.data(),
                    GL_STATIC_DRAW);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    f->glEnableVertexAttribArray(0);

    f->glUseProgram(_gizmos_program);
    f->glUniformMatrix4fv(f->glGetUniformLocation(_gizmos_program, "view"),
                          1,
                          GL_FALSE,
                          _view_matrix.data.data());
    f->glUniformMatrix4fv(
        f->glGetUniformLocation(_gizmos_program, "projection"),
        1,
        GL_FALSE,
        _projection_matrix.data.data());
    f->glUniformMatrix4fv(f->glGetUniformLocation(_gizmos_program, "model"),
                          1,
                          GL_FALSE,
                          model_matrix.data());
    f->glUniform3fv(f->glGetUniformLocation(_gizmos_program, "color"),
                    1,
                    reinterpret_cast<float*>(&color));
    f->glDrawElements(GL_LINES, cube_indices.size(), GL_UNSIGNED_INT, 0);
    f->glBindVertexArray(0);
    f->glUseProgram(0);
}

void renderer::load_object(
    std::shared_ptr<project::renderer_component> renderer)
{
    auto* f = checkAndGetGLFunctions();
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

QOpenGLFunctions_3_3_Core* renderer::checkAndGetGLFunctions()
{
    logger->debug("Trying to get OpenGL 3.3 functions");
    QOpenGLFunctions_3_3_Core* f =
        QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_3_3_Core>(
            QOpenGLContext::currentContext());

    if (!f)
    {
        logger->error("OpenGL functions are not available");
        throw std::runtime_error("OpenGL functions are not available");
    }
    return f;
}

unsigned renderer::load_shader_from_file(std::string_view path)
{
    std::string extension { path.substr(path.find_last_of(".") + 1) };
    int shader_type = -1;
    if (extension == "vert")
    {
        shader_type = GL_VERTEX_SHADER;
    }
    else if (extension == "frag")
    {
        shader_type = GL_FRAGMENT_SHADER;
    }
    else
    {
        logger->error("Unknown shader extension: {}", extension);
        return 0;
    }

    std::ifstream shader_file;
    shader_file.open(path.data());
    if (!shader_file.is_open())
    {
        logger->error("Could not open shader file: {}", path);
        return 0;
    }

    std::string shader_source;
    shader_source.assign(std::istreambuf_iterator<char>(shader_file),
                         std::istreambuf_iterator<char>());
    logger->debug("Loading shader from file: {}", path);
    return load_shader_from_string(shader_source, shader_type);
}

unsigned renderer::load_shader_from_string(std::string_view shader_source,
                                           int shader_type)
{
    auto const* shader_source_ptr = shader_source.data();

    auto f = checkAndGetGLFunctions();
    unsigned shader = f->glCreateShader(shader_type);
    f->glShaderSource(shader, 1, &shader_source_ptr, nullptr);
    f->glCompileShader(shader);
    int error_state;
    f->glGetShaderiv(shader, GL_COMPILE_STATUS, &error_state);
    if (!error_state)
    {
        std::string error_log;
        error_log.resize(512);
        f->glGetShaderInfoLog(shader, 512, nullptr, error_log.data());
        logger->error("Error compiling shader: {}", error_log);
        return 0;
    }
    return shader;
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
