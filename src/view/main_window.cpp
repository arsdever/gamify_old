#include <stdafx_qt>

#include "view/main_window.hpp"

#include "view/scene_view.hpp"
#include <project/project.hpp>
#include <project/scene.hpp>

namespace g::view
{

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow { parent }
    , _project { std::make_shared<project::project>("Dummy project") }
    , _scene { std::make_shared<project::scene>("Dummy scene") }
    , _sceneWidget { new SceneView { _scene } }
    , _view { new QWidget }
{
    _project->add_scene(_scene);
    auto dock = new QDockWidget("Scene explorer", this);
    dock->setWidget(_sceneWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    setCentralWidget(_view);

    setMenuBar(new QMenuBar);
    QMenu* sceneMenu = menuBar()->addMenu("Scene");
    sceneMenu->addActions(_sceneWidget->actions());
}

} // namespace g::view