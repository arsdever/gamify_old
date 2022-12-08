#include <stdafx_qt>

#include "common/logger.hpp"
#include "spdlog/details/registry.h"
#include "view/logging/logger.hpp"
#include "view/logging/logger_model.hpp"
#include "view/logging/qt_logger_sink.hpp"
#include "view/main_window.hpp"

int main(int argc, char** argv)
{
    g::common::configure_logger(argc, argv);
    // g::view::redirect_qt_messages_to_logger();
    g::view::logger_model* model = new g::view::logger_model;
    spdlog::details::registry::instance().apply_all(
        [ model ](g::common::logger_ptr logger)
        {
        logger->sinks().emplace_back(
            std::make_shared<g::view::qt_logger_sink_mt>(*model));
    });

    QApplication app(argc, argv);

    g::view::MainWindow window;
    window.show();
    window.resize(800, 600);

    QTableView view;
    view.setModel(model);
    view.show();

    return app.exec();
}