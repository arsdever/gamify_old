#include <stdafx_qt>

#include "view/scene_view.hpp"

#include "view/scene_model.hpp"
#include <project/project.hpp>
#include <project/scene.hpp>

namespace g::view
{

SceneView::SceneView(project::scene_ptr project, QWidget* parent)
    : QTreeView { parent }
    , _scene { project }
{
}

} // namespace g::view