#include <QBoxLayout>
#include <QGridLayout>
#include <QLabel>

#include "ui/properties_panel.hpp"

#include "project/object.hpp"
#include "project/transform_component.hpp"

namespace g::ui
{

PropertiesPanel::PropertiesPanel(QWidget* parent)
    : QWidget { parent }
{
    setLayout(new QGridLayout);
}

PropertiesPanel::~PropertiesPanel() = default;

void PropertiesPanel::showPropertiesOf(std::shared_ptr<project::object> object)
{
    clear();

    auto transform = object->transform();
    if (transform)
    {
        QGridLayout* gridLayout = qobject_cast<QGridLayout*>(layout());

        gridLayout->addWidget(new QLabel("Position"), 0, 0);
        gridLayout->addWidget(new QLabel("Rotation"), 1, 0);
        gridLayout->addWidget(new QLabel("Scale"), 2, 0);

        common::vector3f pos = transform->position();

        gridLayout->addWidget(new QLabel(QString("X: %1").arg(pos.x())), 0, 1);
        gridLayout->addWidget(new QLabel(QString("Y: %1").arg(pos.y())), 0, 2);
        gridLayout->addWidget(new QLabel(QString("Z: %1").arg(pos.z())), 0, 3);

        common::quaternion rot = transform->rotation();

        gridLayout->addWidget(new QLabel(QString("X: %1").arg(rot.x)), 1, 1);
        gridLayout->addWidget(new QLabel(QString("Y: %1").arg(rot.y)), 1, 2);
        gridLayout->addWidget(new QLabel(QString("Z: %1").arg(rot.z)), 1, 3);
        gridLayout->addWidget(new QLabel(QString("W: %1").arg(rot.w)), 1, 4);

        common::vector3f scale = transform->scale();

        gridLayout->addWidget(
            new QLabel(QString("X: %1").arg(scale.x())), 2, 1);
        gridLayout->addWidget(
            new QLabel(QString("Y: %1").arg(scale.y())), 2, 2);
        gridLayout->addWidget(
            new QLabel(QString("Z: %1").arg(scale.z())), 2, 3);

        gridLayout->addItem(
            new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding),
            3,
            0);
        gridLayout->addItem(
            new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum),
            3,
            5);

        setLayout(gridLayout);
    }
}

void PropertiesPanel::clear()
{
    QGridLayout* gridLayout = qobject_cast<QGridLayout*>(layout());
    if (gridLayout)
    {
        QLayoutItem* item;
        while ((item = gridLayout->takeAt(0)) != nullptr)
        {
            delete item->widget();
            delete item;
        }
    }
}

} // namespace g::ui
