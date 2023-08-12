#pragma once

#include <QWidget>
#include <memory>

namespace g::project
{
class object;
}

namespace g::ui
{

class PropertiesPanel : public QWidget
{
    Q_OBJECT
public:
    PropertiesPanel(QWidget* parent = nullptr);
    ~PropertiesPanel() override;

    void showPropertiesOf(std::shared_ptr<project::object> object);

	private:
    void clear();
};

} // namespace g::ui
