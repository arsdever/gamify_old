#pragma once

#include <project/prototypes.hpp>

namespace g::view
{

class ProjectWidget : public QTreeWidget
{
    Q_OBJECT

public:
    ProjectWidget(QWidget* parent = nullptr);

    void newProject();
    void newScene();

private:
    project::project_ptr _project;
};

} // namespace g::view