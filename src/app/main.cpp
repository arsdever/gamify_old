#include <stdafx_qt>

#include "common/logger.hpp"
#include "view/logging/logger.hpp"
#include "view/main_window.hpp"

int main(int argc, char** argv)
{
    g::common::configure_logger(argc, argv);
    g::view::redirect_qt_messages_to_logger();

    QApplication app(argc, argv);

    g::view::MainWindow window;
    window.show();
    window.resize(800, 600);

    return app.exec();
}