#pragma once

#include <QTreeView>

namespace g::project
{
class scene;
class object;
}

namespace g::ui
{

class SceneView : public QTreeView
{
    Q_OBJECT

public:
    SceneView(std::shared_ptr<project::scene> scene, QWidget* parent = nullptr);

    void onContextMenu(const QPoint& pos);

signals:
    void objectActivated(std::shared_ptr<project::object> object);

private:
    std::shared_ptr<project::scene> _scene;
};

} // namespace g::ui
