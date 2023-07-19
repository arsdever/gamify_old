#pragma once

#include <QMainWindow>

class QPlainTextEdit;

namespace g::project
{
class project;
class scene;
} // namespace g::project

namespace g::ui
{

class AssetManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private:
    void initializeDockWidgets();

    void initializeProjectExplorerDockWidget();
    void initializeLoggerDockWidget();

private:
    std::shared_ptr<project::project> _project;
    std::shared_ptr<project::scene> _scene;
    // ui::AssetManager* _assetManager;

    QWidget* _sceneWidget;
    QWidget* _viewport;

    QWidget* _shaderEditor;
    QPlainTextEdit* _vertexShaderEditor;
    QPlainTextEdit* _fragmentShaderEditor;
};

} // namespace g::ui
