#pragma once

#include <project/prototypes.hpp>

namespace g::view
{

class SceneModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum Role
    {
        ObjectRole = Qt::UserRole + 1,
    };

public:
    SceneModel(project::scene_ptr scene, QObject* parent = nullptr);
    ~SceneModel() override;

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const override;
    QModelIndex index(int row,
                      int column,
                      const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

private:
    project::object_ptr objectAtIndex(const QModelIndex& index) const;

private:
    mutable std::unordered_map<void*, QModelIndex> _indexMap;
    project::scene_ptr _scene;
};

} // namespace g::view