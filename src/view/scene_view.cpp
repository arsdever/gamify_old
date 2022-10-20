#include <stdafx_qt>

#include "view/scene_view.hpp"

#include "view/scene_model.hpp"
#include <project/project.hpp>
#include <project/scene.hpp>

namespace g::view
{

SceneView::SceneView(project::scene_ptr scene, QWidget* parent)
    : QTreeView { parent }
    , _scene { scene }
{
    setModel(new SceneModel(_scene));
    QAction* addObjectAction = new QAction { "New empty", this };
    connect(addObjectAction,
            &QAction::triggered,
            this,
            [ this ] { _scene->create_object("Unnamed"); });
    addAction(addObjectAction);
}

} // namespace g::view