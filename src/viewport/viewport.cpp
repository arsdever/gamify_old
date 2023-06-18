#include <stdafx>

#include "viewport/viewport.hpp"

#include <QOpenGLFunctions>

namespace g::viewport
{

Viewport::Viewport(QWidget* parent)
	: QOpenGLWidget(parent)
{
}

Viewport::~Viewport() = default;

void Viewport::initializeGL()
{
	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
	f->glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void Viewport::resizeGL(int w, int h)
{
	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
	f->glViewport(0, 0, w, h);
}

void Viewport::paintGL()
{
	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
	f->glClear(GL_COLOR_BUFFER_BIT);
}

} // namespace g::viewport
