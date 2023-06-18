#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVersionFunctionsFactory>
#include <stdafx>

#include "viewport/viewport.hpp"

#include "common/logger.hpp"

namespace g::viewport
{

namespace
{
common::logger_ptr logger = common::get_logger("viewport");
}

Viewport::Viewport(QWidget* parent)
    : QOpenGLWidget(parent)
{
}

Viewport::~Viewport() = default;

void Viewport::updateVertexShader(std::string_view source)
{
    _vertexShaderSource = source;
    updateShader(source, _vertexShader);
}

void Viewport::updateFragmentShader(std::string_view source)
{
    _fragmentShaderSource = source;
    updateShader(source, _fragmentShader);
}

void Viewport::updateShaderProgram()
{
    auto* f = checkAndGetGLFunctions();

    if (!_shaderProgram)
    {
        _shaderProgram = f->glCreateProgram();
    }

    unsigned int success;
    f->glAttachShader(_shaderProgram, _vertexShader);

    success = f->glGetError();
    if (!success)
    {
        logger->error("Failed to attach vertex shader. Error code {}", success);
        return;
    }

    f->glAttachShader(_shaderProgram, _fragmentShader);

    success = f->glGetError();
    if (!success)
    {
        logger->error("Failed to attach fragment shader. Error code {}",
                      success);
        return;
    }

    f->glLinkProgram(_shaderProgram);

    f->glGetProgramiv(_shaderProgram, GL_LINK_STATUS, (int*)&success);
    if (!success)
    {
        char infoLog[ 512 ];
        f->glGetProgramInfoLog(_shaderProgram, 512, nullptr, infoLog);
        logger->error(
            "Failed to link shader program. Error code {}. Info log: {}",
            success,
            infoLog);
        return;
    }

    f->glValidateProgram(_shaderProgram);
    update();
}

void Viewport::initializeGL()
{
    float points[] = {
        -0.5f, -0.5f, 0.0f, // left
        0.5f,  -0.5f, 0.0f, // right
        0.0f,  0.5f,  0.0f  // top
    };

    auto* f = checkAndGetGLFunctions();
    f->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    f->glGenBuffers(1, &VBO);
    f->glGenVertexArrays(1, &VAO);

    f->glBindVertexArray(VAO);
    f->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    f->glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    f->glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    f->glEnableVertexAttribArray(0);
}

void Viewport::resizeGL(int w, int h)
{
    auto* f = checkAndGetGLFunctions();
    f->glViewport(0, 0, w, h);
}

void Viewport::paintGL()
{
    auto* f = checkAndGetGLFunctions();
    f->glClear(GL_COLOR_BUFFER_BIT);
    f->glUseProgram(_shaderProgram);
    f->glBindVertexArray(VAO);
    f->glDrawArrays(GL_TRIANGLES, 0, 3);
    logger->debug("Viewport painted");
}

QOpenGLFunctions_3_3_Core* Viewport::checkAndGetGLFunctions()
{
    logger->debug("Trying to get OpenGL 3.3 functions");
    QOpenGLFunctions_3_3_Core* f =
        QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_3_3_Core>(
            context());
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

void Viewport::updateShader(std::string_view source, unsigned int& shader)
{
    auto* f = checkAndGetGLFunctions();

    if (!shader)
    {
        shader = f->glCreateShader(GL_VERTEX_SHADER);
    }

    const char* sourcePtr = source.data();
    f->glShaderSource(shader, 1, &sourcePtr, nullptr);
    f->glCompileShader(shader);

    unsigned int success;
    f->glGetShaderiv(shader, GL_COMPILE_STATUS, (int*)&success);
    if (!success)
    {
        std::string infoLog;
        infoLog.resize(512);
        f->glGetShaderInfoLog(shader, 512, nullptr, infoLog.data());
        logger->error("Failed to compile shader. Error code {}. Info log: {}",
                      success,
                      infoLog);
        return;
    }

    updateShaderProgram();
}

} // namespace g::viewport
