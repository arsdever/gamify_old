#include "ui/scene_model.hpp"

#include "project/object.hpp"
#include "project/project.hpp"
#include "project/resource_manager.hpp"
#include "project/scene.hpp"

namespace g::ui
{

SceneModel::SceneModel(std::shared_ptr<project::scene> scene, QObject* parent)
    : QAbstractItemModel(parent)
    , _scene(std::move(scene))
{
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

    if (!parent.isValid()) // pick a root object
    {
        auto rootObjectsUuid = _scene->objects();

        if (row > rootObjectsUuid.size())
        {
            return {};
        }

        auto it = rootObjectsUuid.begin();
        std::advance(it, row);
        auto rootObject =
            project::resource_manager::get_resource_static<project::object>(
                *it);
        std::size_t id = idFromObject(rootObject);
        _indexMap[ id ] =
            std::make_pair(rootObject, createIndex(row, column, id));
        return _indexMap[ id ].second;
    }

    std::shared_ptr<project::object> obj = nullptr;
    auto parent_obj = objectAtIndex(parent);
    auto it = parent_obj->children_uuid().begin();

    std::advance(it, row);
    auto child =
        project::resource_manager::get_resource_static<project::object>(*it);
    std::size_t id = idFromObject(child);
    _indexMap[ id ] = std::make_pair(child, createIndex(row, column, id));

    return _indexMap[ id ].second;
}

QModelIndex SceneModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
        return {};

    auto object = objectAtIndex(index);
    if (object && object->parent() != nullptr)
    {
        std::size_t id = idFromObject(object->parent());
        return _indexMap[ id ].second;
    }

    return {};
}

int SceneModel::rowCount(const QModelIndex& parent) const
{
    if (!parent.isValid())
        return _scene->objects().size();

    auto parentObject = objectAtIndex(parent);

    if (parentObject)
        return parentObject->children_uuid().size();

    return 0;
}

void SceneModel::addChild(std::shared_ptr<project::object> parent,
                          std::string_view name)
{
    if (parent)
    {
        auto parentIndex = _indexMap[ idFromObject(parent) ].second;
        auto row = parent->children_uuid().size();
        beginInsertRows(parentIndex, row, row);
        project::object::create(name, parent);
        endInsertRows();
    }
    else
    {
        beginInsertRows({}, 0, 0);
        endInsertRows();
    }
}

void SceneModel::removeObject(std::shared_ptr<project::object> object)
{
    std::shared_ptr<project::object> parent = object->parent();
    if (parent)
    {
        auto parentIndex = _indexMap[ idFromObject(parent) ].second;
        auto it = std::find(parent->children_uuid().begin(),
                            parent->children_uuid().end(),
                            object->uuid());
        auto row = std::distance(parent->children_uuid().begin(), it);
        beginRemoveRows(parentIndex, row, row);
        parent->remove_child(object);
        _indexMap.erase(idFromObject(object));
        endRemoveRows();
    }
    else
    {
        beginRemoveRows({}, 0, 0);
        endRemoveRows();
    }
}

std::shared_ptr<project::object>
SceneModel::objectAtIndex(const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return {};
    }

    auto it = _indexMap.find(index.internalId());
    std::shared_ptr<project::object> obj = nullptr;
    if (it == _indexMap.end())
        return {};

    obj = it->second.first.lock();

    if (obj)
        return obj;

    _indexMap.erase(it);
    return {};
}

std::size_t SceneModel::idFromObject(std::shared_ptr<project::object> obj) const
{
    return reinterpret_cast<std::size_t>(obj.get());
}

} // namespace g::ui
