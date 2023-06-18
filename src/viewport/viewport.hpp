#pragma once

#include <QOpenGLWidget>

namespace g::viewport
{

class Viewport : public QOpenGLWidget
{
    Q_OBJECT
public:
    Viewport(QWidget* parent = nullptr);
    ~Viewport() override;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};

} // namespace g::viewport
