#pragma once

#include <project/prototypes.hpp>

namespace g::ui
{

class SceneView : public QTreeView
{
    Q_OBJECT

public:
    SceneView(project::scene_ptr scene, QWidget* parent = nullptr);

    void onContextMenu(const QPoint& pos);

private:
    project::scene_ptr _scene;
};

} // namespace g::ui
