#include "view/scene_model.hpp"

#include <project/project.hpp>
#include <project/scene.hpp>

namespace g::view
{

SceneModel::SceneModel(project::scene_ptr project, QObject* parent)
    : QAbstractItemModel(parent)
    , _scene(std::move(project))
{
}

SceneModel::~SceneModel() = default;

int SceneModel::columnCount(const QModelIndex& parent) const { return 1; }

QVariant SceneModel::data(const QModelIndex& index, int role) const
{
    switch (role)
    {
    default: return {};
    }
}

QModelIndex
SceneModel::index(int row, int column, const QModelIndex& parent) const
{
    return {};
}

QModelIndex SceneModel::parent(const QModelIndex& index) const { return {}; }

int SceneModel::rowCount(const QModelIndex& parent) const { return {}; }

} // namespace g::view