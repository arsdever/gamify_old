#include <QApplication>
#include <QSettings>
#include <QStyleFactory>

#include "common/logger.hpp"
#include "spdlog/details/registry.h"
#include "ui/main_window.hpp"

void configureColorScheme()
{
#ifdef Q_OS_WIN
    QSettings settings("HKEY_CURRENT_"
                       "USER\\Software\\Microsoft\\Windows\\CurrentVersion\\The"
                       "mes\\Personalize",
                       QSettings::NativeFormat);
    if (settings.value("AppsUseLightTheme") == 0)
    {
        qApp->setStyle(QStyleFactory::create("Fusion"));
        QPalette darkPalette;
        QColor darkColor = QColor(45, 45, 45);
        QColor disabledColor = QColor(127, 127, 127);
        darkPalette.setColor(QPalette::Window, darkColor);
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, QColor(18, 18, 18));
        darkPalette.setColor(QPalette::AlternateBase, darkColor);
        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Disabled, QPalette::Text, disabledColor);
        darkPalette.setColor(QPalette::Button, darkColor);
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(
            QPalette::Disabled, QPalette::ButtonText, disabledColor);
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

        darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);
        darkPalette.setColor(
            QPalette::Disabled, QPalette::HighlightedText, disabledColor);

        qApp->setPalette(darkPalette);

        qApp->setStyleSheet(
            "QToolTip { color: #ffffff; background-color: #2a82da; "
            "border: 1px solid white; }");
    }
#endif
}

int main(int argc, char** argv)
{
    Q_INIT_RESOURCE(qspdlog_resources);

    configureColorScheme();
    g::common::configure_logger(argc, argv);
    QApplication app(argc, argv);

    g::ui::MainWindow window;
    window.show();
    window.resize(800, 600);

    return app.exec();
}
