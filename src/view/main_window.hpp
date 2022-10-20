#pragma once

#include <project/prototypes.hpp>
namespace g::view
{

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private:
    project::project_ptr _project;
    project::scene_ptr _scene;

    QWidget* _sceneWidget;
    QWidget* _view;
};

} // namespace g::view