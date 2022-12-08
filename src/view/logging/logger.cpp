#include <stdafx_qt>

#include "view/logging/logger.hpp"

#include "common/logger.hpp"

namespace g::view
{

static void qt_logger_delegate(QtMsgType type,
                               const QMessageLogContext& context,
                               const QString& message)
{
    auto logger = g::common::get_logger("qt");
    switch (type)
    {
    case QtDebugMsg:
        logger->debug(
            "{}:{}: {}", context.file, context.line, message.toStdString());
        break;
    case QtInfoMsg:
        logger->info(
            "{}:{}: {}", context.file, context.line, message.toStdString());
        break;
    case QtWarningMsg:
        logger->warn(
            "{}:{}: {}", context.file, context.line, message.toStdString());
        break;
    case QtCriticalMsg:
        logger->error(
            "{}:{}: {}", context.file, context.line, message.toStdString());
        break;
    case QtFatalMsg:
        logger->critical(
            "{}:{}: {}", context.file, context.line, message.toStdString());
        abort();
    }
}

void redirect_qt_messages_to_logger()
{
    qInstallMessageHandler(qt_logger_delegate);
}

} // namespace g::view::logger