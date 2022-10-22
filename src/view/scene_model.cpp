#include "view/scene_model.hpp"

#include <project/object.hpp>
#include <project/project.hpp>
#include <project/scene.hpp>

namespace g::view
{

SceneModel::SceneModel(project::scene_ptr scene, QObject* parent)
    : QAbstractItemModel(parent)
    , _scene(std::move(scene))
{
    _scene->on_object_added.connect(
        [ this ](project::object_ptr obj)
        {
        layoutChanged();
        obj->on_children_list_changed.connect([ this ]() { layoutChanged(); });
    });
    _scene->on_object_removed.connect([ this ](project::object_ptr)
                                      { layoutChanged(); });
}

SceneModel::~SceneModel() = default;

int SceneModel::columnCount(const QModelIndex& parent) const { return 1; }

QVariant
SceneModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case 0: return "Name";
        }
    }
    return QVariant();
}

QVariant SceneModel::data(const QModelIndex& index, int role) const
{
    switch (role)
    {
    case Qt::DisplayRole:
        return QString::fromStdString(objectAtIndex(index)->name());
    case Role::ObjectRole: return QVariant::fromValue(objectAtIndex(index));
    default: return {};
    }
}

QModelIndex
SceneModel::index(int row, int column, const QModelIndex& parent) const
{
    if (row < 0 || column < 0 || row >= rowCount(parent) ||
        column >= columnCount(parent))
        return {};

    project::object_ptr obj = nullptr;
    std::list<project::object_ptr>::const_iterator it;
    if (!parent.isValid())
    {
        it = _scene->objects().begin();
    }
    else
    {
        auto parent_obj = objectAtIndex(parent);
        it = parent_obj->children().begin();
    }

    std::advance(it, row);
    _indexMap.emplace(it->get(), createIndex(row, column, it->get()));

    return _indexMap[ it->get() ];
}

QModelIndex SceneModel::parent(const QModelIndex& index) const
{
    auto object = objectAtIndex(index);
    if (object->parent() != nullptr)
    {
        return _indexMap[ object->parent().get() ];
    }

    return {};
}

int SceneModel::rowCount(const QModelIndex& parent) const
{
    auto parentObject = objectAtIndex(parent);
    if (!parentObject)
        return _scene->objects().size();

    return parentObject->children().size();
}

project::object_ptr SceneModel::objectAtIndex(const QModelIndex& index) const
{
    if (index.isValid())
    {
        return static_cast<project::object*>(index.internalPointer())
            ->get_ptr();
    }

    return nullptr;
}

} // namespace g::view
