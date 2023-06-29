#include <stdafx>

#include "ui/scene_model.hpp"

#include <project/object.hpp>
#include <project/project.hpp>
#include <project/scene.hpp>

namespace g::ui
{

SceneModel::SceneModel(project::scene_ptr scene, QObject* parent)
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
    if (!row && !column && !parent.isValid())
    {
        std::size_t id = idFromObject(_scene);
        _indexMap[ id ] = std::make_pair(_scene, createIndex(row, column, id));
        return _indexMap[ id ].second;
    }

    if (row < 0 || column < 0 || row >= rowCount(parent) ||
        column >= columnCount(parent))
        return {};

    project::object_ptr obj = nullptr;
    std::list<project::object_ptr>::const_iterator it;
    project::object_ptr parent_obj = objectAtIndex(parent);
    it = parent_obj->children().begin();

    std::advance(it, row);
    [[maybe_unused]] project::object_ptr child = *it;
    std::size_t id = idFromObject(*it);
    _indexMap[ id ] = std::make_pair(*it, createIndex(row, column, id));

    return _indexMap[ id ].second;
}

QModelIndex SceneModel::parent(const QModelIndex& index) const
{
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
        return 1;

    auto parentObject = objectAtIndex(parent);

    if (parentObject)
        return parentObject->children().size();

    return 0;
}

void SceneModel::addChild(project::object_ptr parent, std::string_view name)
{
    if (parent)
    {
        auto parentIndex = _indexMap[ idFromObject(parent) ].second;
        auto row = parent->children().size();
        beginInsertRows(parentIndex, row, row);
        parent->add_child(project::object::create(name));
        endInsertRows();
    }
    else
    {
        beginInsertRows({}, 0, 0);
        endInsertRows();
    }
}

void SceneModel::removeObject(project::object_ptr object)
{
    project::object_ptr parent = object->parent();
    if (parent)
    {
        auto parentIndex = _indexMap[ idFromObject(parent) ].second;
        auto it = std::find(
            parent->children().begin(), parent->children().end(), object);
        auto row = std::distance(parent->children().begin(), it);
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

project::object_ptr SceneModel::objectAtIndex(const QModelIndex& index) const
{
    if (index.isValid())
    {
        auto it = _indexMap.find(index.internalId());
        project::object_ptr obj = nullptr;
        if (it == _indexMap.end())
            return {};

        obj = it->second.first.lock();

        if (obj)
            return obj;

        _indexMap.erase(it);
        return {};
    }

    return _scene;
}

std::size_t SceneModel::idFromObject(project::object_ptr obj) const
{
    return reinterpret_cast<std::size_t>(obj.get());
}

} // namespace g::ui
