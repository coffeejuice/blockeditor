#ifndef BLOCKSLISTMODEL_H
#define BLOCKSLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QQmlEngine>

class BlocksListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("BlocksListModel must be instantinated in C++")

    // QStringList blocksTypes;
    QList<QJsonObject> blocksContents;

public:
    enum BlocksRoles {
        // BlockType = Qt::UserRole + 1,
        BlockContent = Qt::UserRole + 1,
    };

    explicit BlocksListModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    Q_INVOKABLE bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationRow) override;
    Q_INVOKABLE bool insertRows(int row, int count, const QModelIndex &parent) override;
    Q_INVOKABLE bool removeRows(int row, int count, const QModelIndex &parent) override;
    Q_INVOKABLE bool appendRow(int rowNumber);
    Q_INVOKABLE bool clearModel(int rowNumber);
};

#endif // BLOCKSLISTMODEL_H
