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
    _scene->on_object_added.connect([ this ](project::object_ptr)
                                    { layoutChanged(); });
}

SceneModel::~SceneModel() = default;

int SceneModel::columnCount(const QModelIndex& parent) const { return 1; }

QVariant SceneModel::data(const QModelIndex& index, int role) const
{
    switch (role)
    {
    case Qt::DisplayRole:
        return QString::fromStdString(
            static_cast<project::object*>(index.internalPointer())->name());
    default: return {};
    }
}

QModelIndex
SceneModel::index(int row, int column, const QModelIndex& parent) const
{
    auto parentObject = static_cast<project::object*>(parent.internalPointer());

    std::list<project::object_ptr>::const_iterator it;
    if (parentObject == nullptr)
    {
        it = _scene->objects().begin();
    }
    else
    {
        it = parentObject->children().begin();
    }

    int idx = row;
    while (idx--)
    {
        ++it;
    }

    return createIndex(row, column, it->get());
}

QModelIndex SceneModel::parent(const QModelIndex& index) const
{
    auto object = static_cast<project::object*>(index.internalPointer());
    if (object->parent() != nullptr) { }
    return {};
}

int SceneModel::rowCount(const QModelIndex& parent) const
{
    auto parentObject = static_cast<project::object*>(parent.internalPointer());
    if (!parentObject)
        return _scene->objects().size();

    return parentObject->children().size();
}

} // namespace g::view