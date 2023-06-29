#include <QFile>

#include "ui/asset_manager.hpp"

namespace g::ui
{

AssetManager::AssetManager() = default;

AssetManager::~AssetManager() = default;

void AssetManager::loadAsset(std::string_view file_path)
{
    beginInsertRows(QModelIndex(), _assets.size(), _assets.size());
    _assets.emplace_back(std::make_unique<Asset>(file_path));
    endInsertRows();
}

void AssetManager::createAsset(std::string_view file_path)
{
    QFile file(QString::fromStdString(file_path.data()));
    file.open(QIODevice::WriteOnly);
    file.close();
    loadAsset(file_path);
}

int AssetManager::rowCount(const QModelIndex& parent) const
{
    return _assets.size();
}

QVariant AssetManager::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        return QString::fromStdString(_assets[ index.row() ]->name());
    } else if (role == Qt::DecorationRole)
    {
        return QIcon(QString::fromStdString(_assets[ index.row() ]->name()));
    }
    return QVariant();
}

} // namespace g::ui
