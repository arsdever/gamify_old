#pragma once

#include <project/prototypes.hpp>
#include <engine/prototypes.hpp>

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
    engine::engine_ptr _engine;
};

} // namespace g::view