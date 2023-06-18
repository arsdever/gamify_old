#pragma once

#include <project/prototypes.hpp>

class QTextEdit;

namespace g::ui
{

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private:
    void initializeDockWidgets();

    void initializeProjectExplorerDockWidget();
    void initializeLoggerDockWidget();

private:
    project::project_ptr _project;
    project::scene_ptr _scene;

    QWidget* _sceneWidget;
    QWidget* _viewport;

    QWidget* _shaderEditor;
    QTextEdit* _vertexShaderEditor;
    QTextEdit* _fragmentShaderEditor;
};

} // namespace g::ui
