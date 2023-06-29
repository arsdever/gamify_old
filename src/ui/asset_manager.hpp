#pragma once

#include <QAbstractListModel>

#include "ui/asset.hpp"
#include <assimp/Importer.hpp>

namespace g::ui
{

class AssetManager : public QAbstractListModel
{
public:
    AssetManager();
    ~AssetManager();

    void loadAsset(std::string_view file_path);
    void createAsset(std::string_view file_path);

#pragma region QAbstractListModel
public:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const override;
#pragma endregion

private:
    std::vector<std::unique_ptr<Asset>> _assets;
};

} // namespace g::ui
