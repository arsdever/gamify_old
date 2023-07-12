#pragma once

#include <QAbstractItemModel>

namespace g::project
{
class object;
class scene;
} // namespace g::project

Q_DECLARE_METATYPE(std::shared_ptr<g::project::object>)

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
    SceneModel(std::shared_ptr<project::scene> scene,
               QObject* parent = nullptr);
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

    void addChild(std::shared_ptr<project::object> parent,
                  std::string_view name);
    void removeObject(std::shared_ptr<project::object> object);

private:
    std::shared_ptr<project::object>
    objectAtIndex(const QModelIndex& index) const;
    std::size_t idFromObject(std::shared_ptr<project::object> obj) const;

private:
    mutable std::unordered_map<
        std::size_t,
        std::pair<std::weak_ptr<project::object>, QModelIndex>>
        _indexMap;
    std::shared_ptr<project::scene> _scene;
};

} // namespace g::ui
