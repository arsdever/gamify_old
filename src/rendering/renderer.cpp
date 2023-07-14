#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVersionFunctionsFactory>
#include <fstream>

#include "renderer.hpp"

#include "common/logger.hpp"
#include "common/matrix.hpp"
#include "project/assets/material_asset.hpp"
#include "project/assets/mesh_asset.hpp"
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

private:
    QOpenGLFunctions_3_3_Core* checkAndGetGLFunctions();

    unsigned modelViewProjectionShader;
    unsigned lightingFragmentShader;
    unsigned default_vert;
    unsigned default_frag;
};

renderer::renderer() = default;

renderer::~renderer() = default;

void renderer::initialize()
{
    logger->debug("Initializing renderer");

    auto* f = checkAndGetGLFunctions();

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
                logger->warn("Could not find uniform {}", property.first);
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

        f->glBindVertexArray(render_context->vao);
        f->glDrawElements(
            GL_TRIANGLES, render_context->index_count, GL_UNSIGNED_INT, 0);
        f->glBindVertexArray(0);
        f->glUseProgram(0);
    }
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
        std::string vshader_source = material->vertex_shader_source();
        std::string fshader_source = material->fragment_shader_source();

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
