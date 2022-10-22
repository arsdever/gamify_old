#pragma once

#include <project/prototypes.hpp>
#include <unordered_map>

namespace g::view
{

class SceneModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    SceneModel(project::scene_ptr scene, QObject* parent = nullptr);
    ~SceneModel() override;

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const override;
    QModelIndex index(int row,
                      int column,
                      const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

private:
    std::unordered_map<uint32_t, QModelIndex> _indexMapping;
    project::scene_ptr _scene;
};

} // namespace g::view

