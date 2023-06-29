#pragma once

#include <project/prototypes.hpp>

class QPlainTextEdit;

namespace g::ui
{

class AssetManager;

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
    ui::AssetManager* _assetManager;

    QWidget* _sceneWidget;
    QWidget* _viewport;

    QWidget* _shaderEditor;
    QPlainTextEdit* _vertexShaderEditor;
    QPlainTextEdit* _fragmentShaderEditor;
};

} // namespace g::ui
