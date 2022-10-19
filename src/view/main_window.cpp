#include <stdafx_qt>

#include "view/main_window.hpp"

#include "view/scene_view.hpp"

namespace g::view
{

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow { parent }
    , _sceneWidget { new SceneView { nullptr } }
    , _view { new QWidget }
{
    auto dock = new QDockWidget("Scene explorer", this);
    dock->setWidget(_sceneWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    setCentralWidget(_view);

    setMenuBar(new QMenuBar);
    QMenu* sceneMenu = menuBar()->addMenu("Scene");
    sceneMenu->addActions(_sceneWidget->actions());
}

} // namespace g::view