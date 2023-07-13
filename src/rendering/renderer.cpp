#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVersionFunctionsFactory>

#include "renderer.hpp"

#include "common/logger.hpp"
#include "project/material.hpp"
#include "project/mesh_component.hpp"
#include "project/object.hpp"
#include "project/project.hpp"
#include "project/renderer_component.hpp"
#include "project/resource_manager.hpp"
#include "project/scene.hpp"
#include <assimp/scene.h>

namespace g::rendering
{

renderer::~renderer() = default;

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

    void initialize(QOpenGLContext* context) override;
    void render(std::shared_ptr<project::renderer_component> renderer) override;
    virtual void
    load_object(std::shared_ptr<project::renderer_component> renderer) override;

private:
    QOpenGLFunctions_3_3_Core* checkAndGetGLFunctions();

private:
    QOpenGLContext* _context { nullptr };
};

renderer::renderer() = default;

renderer::~renderer() = default;

void renderer::initialize(QOpenGLContext* context)
{
    logger->debug("Initializing renderer");
    _context = context;
}

void renderer::render(std::shared_ptr<project::renderer_component> renderer)
{
    auto* f = checkAndGetGLFunctions();
    if (renderer->enabled())
    {
        auto render_context =
            static_cast<opengl_3_3_render_context*>(renderer->renderContext());
        if (!render_context)
        {
            logger->error("Render context is null");
            return;
        }

        f->glUseProgram(render_context->program);
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

    auto mesh = renderer->get_component<project::mesh_component>();

    context->vertex_count = mesh->vertex_coordinates().size();
    context->index_count = mesh->vertex_indices().size();

    f->glGenVertexArrays(1, &context->vao);
    f->glGenBuffers(1, &context->vbo);
    f->glGenBuffers(1, &context->ebo);

    f->glBindVertexArray(context->vao);
    f->glBindBuffer(GL_ARRAY_BUFFER, context->vbo);
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, context->ebo);

    f->glBufferData(GL_ARRAY_BUFFER,
                    mesh->vertex_coordinates().size() * sizeof(float) * 3,
                    mesh->vertex_coordinates().data(),
                    GL_STATIC_DRAW);
    f->glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                    mesh->vertex_indices().size() * sizeof(unsigned),
                    mesh->vertex_indices().data(),
                    GL_STATIC_DRAW);

    f->glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    f->glEnableVertexAttribArray(0);
    f->glBindVertexArray(0);

    context->program = f->glCreateProgram();
    context->vshader = f->glCreateShader(GL_VERTEX_SHADER);
    context->fshader = f->glCreateShader(GL_FRAGMENT_SHADER);

    auto* material = renderer->material();
    if (material)
    {
        std::string vshader_source = material->vertex_shader_source();
        std::string fshader_source = material->fragment_shader_source();

        const char* vsh = vshader_source.c_str();
        const char* fsh = fshader_source.c_str();

        f->glShaderSource(context->vshader, 1, &vsh, nullptr);
        f->glShaderSource(context->fshader, 1, &fsh, nullptr);

        f->glCompileShader(context->vshader);
        f->glCompileShader(context->fshader);

        f->glAttachShader(context->program, context->vshader);
        f->glAttachShader(context->program, context->fshader);

        f->glLinkProgram(context->program);
    }

    renderer->setRenderContext(
        std::unique_ptr<project::render_context>(context));
}

// void renderer::draw_asset(std::shared_ptr<project::asset> asset,
//                           QOpenGLContext* context)
// {
//     logger->debug("Drawing asset");

//     // Construct mesh_asset from fbx
//     draw_mesh(asset->data<project::asset::asset_type::mesh_asset>(),
//     context);
// }

// void renderer::draw_mesh(project::mesh_asset* mesh_asset,
//                          QOpenGLContext* context)
// {
//     logger->debug("Drawing mesh_asset");

//     auto* f = checkAndGetGLFunctions();

//     f->glViewport(0, 0, 128, 128);
//     unsigned prog = f->glCreateProgram();
//     unsigned vert, frag;

//     vert = f->glCreateShader(GL_VERTEX_SHADER);
//     const char* vsh = R"(#version 330 core
// layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aNormal;
// out vec3 ourColor;
// void main()
// {
//     gl_Position = vec4(aPos, 1.0);
//     ourColor = aNormal;
// })";

//     f->glShaderSource(vert, 1, &vsh, nullptr);
//     f->glCompileShader(vert);

//     frag = f->glCreateShader(GL_FRAGMENT_SHADER);
//     const char* fsh = R"(#version 330 core
// out vec4 FragColor;
// in vec3 ourColor;
// void main()
// {
//     FragColor = vec4(ourColor, 1.0);
// })";

//     f->glShaderSource(frag, 1, &fsh, nullptr);
//     f->glCompileShader(frag);

//     f->glAttachShader(prog, vert);
//     f->glAttachShader(prog, frag);
//     f->glLinkProgram(prog);

//     unsigned vbo;
//     unsigned vao;
//     unsigned ebo;
//     f->glGenBuffers(1, &vbo);
//     f->glGenVertexArrays(1, &vao);
//     f->glGenBuffers(1, &ebo);

//     std::vector<std::array<float, 6>> points = mesh_asset->vertices();

//     f->glBindVertexArray(vao);
//     f->glBindBuffer(GL_ARRAY_BUFFER, vbo);
//     f->glBufferData(GL_ARRAY_BUFFER,
//                     points.size() * 6 * sizeof(float),
//                     points.data(),
//                     GL_STATIC_DRAW);

//     std::vector<unsigned> indices = mesh_asset->indices();
//     f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//     f->glBufferData(GL_ELEMENT_ARRAY_BUFFER,
//                     indices.size() * sizeof(unsigned),
//                     indices.data(),
//                     GL_STATIC_DRAW);

//     f->glVertexAttribPointer(
//         0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
//     f->glVertexAttribPointer(1,
//                              3,
//                              GL_FLOAT,
//                              GL_FALSE,
//                              6 * sizeof(float),
//                              (void*)(3 * sizeof(float)));
//     f->glEnableVertexAttribArray(0);

//     f->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//     f->glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
//     f->glBindVertexArray(0);
//     f->glDeleteBuffers(1, &vbo);
//     f->glDeleteVertexArrays(1, &vao);
//     f->glDeleteBuffers(1, &ebo);
//     f->glDeleteShader(vert);
//     f->glDeleteShader(frag);
//     f->glDeleteProgram(prog);
// }

// void renderer::compile_shader(project::shader_asset* shader_asset)
// {
//     if (_compiled_shaders.find(shader_asset) != _compiled_shaders.end())
//     {
//         logger->debug("Shader already compiled");
//         return;
//     }

//     auto* f = checkAndGetGLFunctions();
//     unsigned prog = f->glCreateProgram();
//     unsigned vert, frag;

//     vert = f->glCreateShader(GL_VERTEX_SHADER);
//     const char* vsh =
//         shader_asset->program<project::shader_asset::shader_type::vertex>()
//             .data();
//     f->glShaderSource(vert, 1, &vsh, nullptr);
//     f->glCompileShader(vert);

//     // handle errors
//     int success;
//     std::string infoLog;
//     infoLog.resize(512);
//     f->glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
//     if (!success)
//     {
//         f->glGetShaderInfoLog(vert, 512, nullptr, infoLog.data());
//         logger->error("Vertex shader_asset compilation failed: {}", infoLog);
//         return;
//     }

//     frag = f->glCreateShader(GL_FRAGMENT_SHADER);
//     const char* fsh =
//         shader_asset->program<project::shader_asset::shader_type::fragment>()
//             .data();
//     f->glShaderSource(frag, 1, &fsh, nullptr);
//     f->glCompileShader(frag);

//     // handle errors
//     f->glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
//     if (!success)
//     {
//         f->glGetShaderInfoLog(frag, 512, nullptr, infoLog.data());
//         logger->error("Fragment shader_asset compilation failed: {}",
//         infoLog); return;
//     }

//     f->glAttachShader(prog, vert);
//     f->glAttachShader(prog, frag);
//     f->glLinkProgram(prog);

//     // handle errors
//     f->glGetProgramiv(prog, GL_LINK_STATUS, &success);
//     if (!success)
//     {
//         f->glGetProgramInfoLog(prog, 512, nullptr, infoLog.data());
//         logger->error("Shader program linking failed: {}", infoLog);
//         return;
//     }

//     _compiled_shaders[ shader_asset ] = prog;
// }

QOpenGLFunctions_3_3_Core* renderer::checkAndGetGLFunctions()
{
    logger->debug("Trying to get OpenGL 3.3 functions");
    QOpenGLFunctions_3_3_Core* f =
        QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_3_3_Core>(
            _context);
    // if (f)
    // {
    //     logger->info("OpenGL 3.3 functions are available");
    //     return f;
    // }

    // f = QOpenGLContext::currentContext()->functions();
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
