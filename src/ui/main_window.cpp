#include <QFileDialog>
#include <QListView>
#include <QPlainTextEdit>
#include <QTabWidget>
#include <QToolBar>
#include <QToolButton>
#include <stdafx_qt>

#include "ui/main_window.hpp"

#include "project/project.hpp"
#include "project/scene.hpp"
#include "qspdlog/qspdlog.hpp"
#include "ui/asset_manager.hpp"
#include "ui/scene_view.hpp"
#include "viewport/viewport.hpp"
#include <spdlog/sinks/sink.h>
#include <spdlog/spdlog.h>

namespace g::ui
{

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow { parent }
    , _project { std::make_shared<project::project>("Dummy project") }
    , _scene { project::scene::create("Dummy scene") }
    , _assetManager { new ui::AssetManager }
    , _viewport { new viewport::Viewport }
    , _shaderEditor { new QTabWidget }
    , _vertexShaderEditor { new QPlainTextEdit }
    , _fragmentShaderEditor { new QPlainTextEdit }
{
    initializeDockWidgets();

    _project->add_scene(_scene);
    setCentralWidget(_viewport);

    _scene->add_child(project::object::create("Dummy object"))
        ->add_child(project::object::create("Dummy child"));

    _scene->add_child(project::object::create("Dummy object 2"))
        ->add_child(project::object::create("Dummy child 2"));

    _scene->add_child(project::object::create("Dummy object 3"))
        ->add_child(project::object::create("Dummy child 3"))
        ->add_child(project::object::create("Dummy child 3.1"));

    setMenuBar(new QMenuBar);
    QMenu* sceneMenu = menuBar()->addMenu("Scene");
    sceneMenu->addActions(_sceneWidget->actions());

    QTabWidget* shaderEditorCasted = qobject_cast<QTabWidget*>(_shaderEditor);
    shaderEditorCasted->addTab(_vertexShaderEditor, "Vertex shader");
    shaderEditorCasted->addTab(_fragmentShaderEditor, "Fragment shader");

    QToolBar* toolbar = addToolBar("Shader editor");
    QToolButton* compileButton = new QToolButton;
    compileButton->setText("Compile");
    toolbar->addWidget(compileButton);

    compileButton->connect(compileButton,
                           &QToolButton::clicked,
                           [ this ]
                           {
        viewport::Viewport* viewport =
            qobject_cast<viewport::Viewport*>(_viewport);
        viewport->updateVertexShader(
            _vertexShaderEditor->toPlainText().toStdString());
        viewport->updateFragmentShader(
            _fragmentShaderEditor->toPlainText().toStdString());
        viewport->updateShaderProgram();
    });

    QDockWidget* shaderEditorDockWidget =
        new QDockWidget("Shader editor", this);
    shaderEditorDockWidget->setWidget(_shaderEditor);
    addDockWidget(Qt::RightDockWidgetArea, shaderEditorDockWidget);

    QDockWidget* assetsDockWidget = new QDockWidget("Assets", this);
    QListView* assetsListView = new QListView;
    assetsDockWidget->setWidget(assetsListView);
    assetsListView->setModel(_assetManager);
    assetsListView->setViewMode(QListView::IconMode);
    assetsListView->setUniformItemSizes(true);
    addDockWidget(Qt::BottomDockWidgetArea, assetsDockWidget);

    QToolButton* addAssetButton = new QToolButton;
    addAssetButton->setText("Add asset");
    toolbar->addWidget(addAssetButton);
    connect(addAssetButton,
            &QToolButton::clicked,
            [ this ]
            {
        QString file_path = QFileDialog::getOpenFileName(
            this, "Open asset", "", "All files (*.*)");
        if (!file_path.isEmpty())
        {
            _assetManager->loadAsset(file_path.toStdString());
        }
    });
}

void MainWindow::initializeDockWidgets()
{
    initializeProjectExplorerDockWidget();
    initializeLoggerDockWidget();
}

void MainWindow::initializeProjectExplorerDockWidget()
{
    auto dock = new QDockWidget("Scene explorer", this);
    _sceneWidget = new SceneView { _scene };
    dock->setWidget(_sceneWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
}

void MainWindow::initializeLoggerDockWidget()
{
    auto loggerDockWidget = new QDockWidget("Logger", this);

    QSpdLog* qspdlog = new QSpdLog(this);
    qspdlog->setAutoScrollPolicy(
        AutoScrollPolicy::AutoScrollPolicyEnabledIfBottom);
    qspdlog->sink()->set_level(spdlog::level::info);
    // register all loggers into the qspldog widget
    auto& registry = spdlog::details::registry::instance();
    registry.apply_all([ qspdlog ](std::shared_ptr<spdlog::logger> logger)
                       { logger->sinks().push_back(qspdlog->sink()); });

    loggerDockWidget->setWidget(qspdlog);
    addDockWidget(Qt::BottomDockWidgetArea, loggerDockWidget);
}

} // namespace g::ui
