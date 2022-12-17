#include <stdafx_qt>

#include "common/logger.hpp"
#include "spdlog/details/registry.h"
#include "view/main_window.hpp"

int main(int argc, char** argv)
{
    Q_INIT_RESOURCE(qspdlog_resources);

    g::common::configure_logger(argc, argv);
    QApplication app(argc, argv);

    g::view::MainWindow window;
    window.show();
    window.resize(800, 600);

    return app.exec();
}