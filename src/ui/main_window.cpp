#include <stdafx_qt>

#include "ui/main_window.hpp"

#include "project/project.hpp"
#include "project/scene.hpp"
#include "qspdlog/qspdlog.hpp"
#include "ui/scene_view.hpp"
#include "viewport/viewport.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/sink.h>

namespace g::ui
{

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow { parent }
    , _project { std::make_shared<project::project>("Dummy project") }
    , _scene { project::scene::create("Dummy scene") }
    , _viewport { new viewport::Viewport }
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
