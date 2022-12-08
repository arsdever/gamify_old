#pragma once

namespace g::view
{

class logger_model : public QAbstractTableModel
{
public:
    struct entry_t
    {
        std::chrono::duration<double> time;
        int level;
        std::string message;
    };

public:
    logger_model(QObject* parent = nullptr);
    ~logger_model() override = default;

    void add_entry(entry_t entry);

#pragma region QAbstractTableModel
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
#pragma endregion

private:
    std::vector<entry_t> _items;
};

} // namespace g::view