#include <stdafx_qt>

#include "view/project_widget.hpp"

#include <project/project.hpp>
#include <project/scene.hpp>

namespace g::view
{

ProjectWidget::ProjectWidget(QWidget* parent)
    : QTreeWidget { parent }
{
    QAction* newProjectAction = new QAction("New Project", this);
    connect(newProjectAction,
            &QAction::triggered,
            this,
            &ProjectWidget::newProject);

    addAction(newProjectAction);

    QAction* newSceneAction = new QAction("New Scene", this);
    connect(
        newSceneAction, &QAction::triggered, this, &ProjectWidget::newScene);

    addAction(newSceneAction);
    newSceneAction->setEnabled(false);
}

void ProjectWidget::newProject()
{
    _project = std::make_shared<project::project>("Untitled");
    _project->signal_name_changed.connect(
        [ this ](std::string_view name) {
        this->setWindowTitle(QString::fromStdString(std::string { name }));
    });
    _project->signal_active_scene_changed.connect(
        [ this ](project::scene_ptr scene)
        {
        this->clear();
        this->addTopLevelItem(new QTreeWidgetItem(
            QStringList(QString::fromStdString(scene->name()))));
    });
    _project->signal_scene_added.connect(
        [ this ](project::scene_ptr scene)
        {
        this->addTopLevelItem(new QTreeWidgetItem(
            QStringList(QString::fromStdString(scene->name()))));
    });

    _project->set_name("New Project");
    (*std::find_if(actions().begin(),
                   actions().end(),
                   [](QAction* action) {
        return action->text() == "New Scene";
    }))->setEnabled(true);
    newScene();
}

void ProjectWidget::newScene()
{
    _project->add_scene(std::make_shared<project::scene>("Untitled"));
}

} // namespace g::view