#include <stdafx_qt>

#include "view/scene_view.hpp"

#include "view/scene_model.hpp"
#include <project/object.hpp>
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

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,
            &QWidget::customContextMenuRequested,
            this,
            &SceneView::onContextMenu);
}

void SceneView::onContextMenu(const QPoint& pos)
{
    QMenu menu { this };
    QAction* newObject = menu.addAction("New empty");

    auto index = indexAt(pos);
    if (index.isValid())
    {
        auto object = static_cast<project::object*>(index.internalPointer())
                          ->shared_from_this();
        if (object)
        {
            connect(newObject,
                    &QAction::triggered,
                    this,
                    [ this, object ]
                    {
                object->add_child(std::make_shared<project::object>(
                    object->name() + " child", object));
                object->on_children_list_changed.connect(
                    [ this ]() { model()->layoutChanged(); });
            });

            QAction* deleteAction = menu.addAction("Delete");
            connect(deleteAction,
                    &QAction::triggered,
                    this,
                    [ this, object ] { object->remove_child(object); });
            menu.exec(mapToGlobal(pos));
        }
    }
    else
    {
        connect(newObject,
                &QAction::triggered,
                this,
                [ this ]
                {
            _scene->create_object("Unnamed", nullptr)
                ->on_children_list_changed.connect(
                    [ this ]() { model()->layoutChanged(); });
        });
        menu.exec(mapToGlobal(pos));
    }
}

} // namespace g::view
