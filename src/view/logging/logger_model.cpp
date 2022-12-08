#include <stdafx_qt>

#include "view/logging/logger_model.hpp"

namespace g::view
{

namespace
{

const char* level_names[] = { "Trace", "Debug",    "Info", "Warning",
                              "Error", "Critical", "Off" };

const char* column_names[] = { "Time", "Level", "Message" };

} // namespace

logger_model::logger_model(QObject* parent)
    : QAbstractTableModel(parent)
{
}

void logger_model::add_entry(entry_t entry)
{
    beginInsertRows(QModelIndex(), _items.size(), _items.size());
    _items.push_back(std::move(entry));
    endInsertRows();
}

int logger_model::rowCount(const QModelIndex& parent) const
{
    return _items.size();
}

int logger_model::columnCount(const QModelIndex& parent) const { return 3; }

QVariant logger_model::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        auto const& item = _items[ index.row() ];
        switch (index.column())
        {
        case 0:
            return QDateTime::fromMSecsSinceEpoch(
                       std::chrono::duration_cast<std::chrono::milliseconds>(
                           item.time)
                           .count())
                .toString();
        case 1: return QString(level_names[ item.level ]);
        case 2: return QString::fromStdString(item.message);
        }
    }
    return QVariant();
}

QVariant logger_model::headerData(int section,
                                  Qt::Orientation orientation,
                                  int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
        return QString(column_names[ section ]);
    return QVariant();
}

} // namespace g::view