#include <stdafx_qt>

#include "ui/scene_view.hpp"

#include "ui/scene_model.hpp"
#include <project/object.hpp>
#include <project/project.hpp>
#include <project/scene.hpp>

namespace g::ui
{

// TODO: Improve counting of the unnamed objects
static int object_index = 0;

SceneView::SceneView(project::scene_ptr scene, QWidget* parent)
    : QTreeView { parent }
    , _scene { scene }
{
    setModel(new SceneModel(_scene));

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,
            &QWidget::customContextMenuRequested,
            this,
            &SceneView::onContextMenu);
}

void SceneView::onContextMenu(const QPoint& pos)
{
    QMenu menu { this };

    auto index = indexAt(pos);
    if (index.isValid())
    {
        QAction* newObject = menu.addAction("New empty");
        QAction* deleteAction = menu.addAction("Delete");

        auto object = model()
                          ->data(index, SceneModel::ObjectRole)
                          .value<project::object_ptr>();
        if (object)
        {
            connect(newObject,
                    &QAction::triggered,
                    this,
                    [ this, object ]
                    {
                qobject_cast<SceneModel*>(model())->addChild(
                    object, object->name() + " child");
            });

            connect(deleteAction,
                    &QAction::triggered,
                    this,
                    [ this, object ] {
                qobject_cast<SceneModel*>(model())->removeObject(object);
            });
            menu.exec(mapToGlobal(pos));
        }
    }
    else
    {
        menu.exec(mapToGlobal(pos));
    }
}

} // namespace g::ui
