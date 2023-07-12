#pragma once

#include <QTreeView>

namespace g::project
{
class scene;
}

namespace g::ui
{

class SceneView : public QTreeView
{
    Q_OBJECT

public:
    SceneView(std::shared_ptr<project::scene> scene, QWidget* parent = nullptr);

    void onContextMenu(const QPoint& pos);

private:
    std::shared_ptr<project::scene> _scene;
};

} // namespace g::ui
