#pragma once

#include <project/prototypes.hpp>

Q_DECLARE_METATYPE(g::project::object_ptr)

namespace g::ui
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

    void addChild(project::object_ptr parent, std::string_view name);
    void removeObject(project::object_ptr object);

private:
    project::object_ptr objectAtIndex(const QModelIndex& index) const;
    std::size_t idFromObject(project::object_ptr obj) const;

private:
    mutable std::unordered_map<std::size_t, std::pair<project::object_wptr, QModelIndex>> _indexMap;
    project::scene_ptr _scene;
};

} // namespace g::ui
