#pragma once

namespace g::view {

class ProjectWidget : public QTreeWidget {
  Q_OBJECT

public:
  ProjectWidget(QWidget *parent = nullptr);
};

} // namespace g::view