#pragma once

#include <QOpenGLWidget>

class QOpenGLFunctions;
class QOpenGLFunctions_3_3_Core;

namespace g::viewport
{

class Viewport : public QOpenGLWidget
{
    Q_OBJECT
public:
    Viewport(QWidget* parent = nullptr);
    ~Viewport() override;

    void updateVertexShader(std::string_view source);
    void updateFragmentShader(std::string_view source);
    void updateShaderProgram();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    QOpenGLFunctions_3_3_Core* checkAndGetGLFunctions();
    void updateShader(std::string_view source,
                      unsigned int& shader,
                      unsigned int shaderType);

private:
    std::string _vertexShaderSource;
    std::string _fragmentShaderSource;

    unsigned int _vertexShader { 0 };
    unsigned int _fragmentShader { 0 };
    unsigned int _shaderProgram { 0 };

    unsigned int VBO { 0 };
    unsigned int VAO { 0 };
    unsigned int EBO { 0 };
};

} // namespace g::viewport
