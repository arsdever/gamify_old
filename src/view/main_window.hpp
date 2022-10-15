#pragma once

namespace g::view {

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);

private:
  QWidget *_projectWidget;
  QWidget *_view;
};

} // namespace g::view