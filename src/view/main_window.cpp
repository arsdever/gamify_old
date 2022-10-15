#include <stdafx_qt>

#include "view/main_window.hpp"

#include "view/project_widget.hpp"

namespace g::view
{

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow { parent }
    , _projectWidget { new ProjectWidget }
    , _view { new QWidget }
{
    auto dock = new QDockWidget("Project", this);
    dock->setWidget(_projectWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    setCentralWidget(_view);
}

} // namespace g::view