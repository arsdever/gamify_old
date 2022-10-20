#pragma once

#include <project/prototypes.hpp>

namespace g::view
{

class SceneView : public QTreeView
{
    Q_OBJECT

public:
    SceneView(project::scene_ptr scene, QWidget* parent = nullptr);

private:
    project::scene_ptr _scene;
};

} // namespace g::view