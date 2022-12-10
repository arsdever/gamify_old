#include <stdafx_qt>

#include "view/main_window.hpp"

#include "project/project.hpp"
#include "project/scene.hpp"
#include "view/logging/logger.hpp"
#include "view/logging/logger_model.hpp"
#include "view/logging/qt_logger_sink.hpp"
#include "view/scene_view.hpp"
#include <spdlog/spdlog.h>

namespace g::view
{

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow { parent }
    , _project { std::make_shared<project::project>("Dummy project") }
    , _scene { project::scene::create("Dummy scene") }
    , _view { new QWidget }
{
    initializeDockWidgets();

    _project->add_scene(_scene);
    setCentralWidget(_view);

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

    QTableView* loggerView = new QTableView;
    loggerDockWidget->setWidget(loggerView);
    auto model = new logger_model;
    loggerView->setModel(model);

    spdlog::details::registry::instance().apply_all(
        [ model ](g::common::logger_ptr logger)
        {
        logger->sinks().emplace_back(
            std::make_shared<g::view::qt_logger_sink_mt>(*model));
    });
    addDockWidget(Qt::BottomDockWidgetArea, loggerDockWidget);
}

} // namespace g::view
