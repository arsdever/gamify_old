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
    , _scene { project::scene::create("Dummy scene") }
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

    _scene->add_child(project::object::create("Dummy object"))
        ->add_child(project::object::create("Dummy child"));

    _scene->add_child(project::object::create("Dummy object 2"))
        ->add_child(project::object::create("Dummy child 2"));

    _scene->add_child(project::object::create("Dummy object 3"))
        ->add_child(project::object::create("Dummy child 3"))
        ->add_child(project::object::create("Dummy child 3.1"));
}

} // namespace g::view