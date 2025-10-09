#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QVariantMap>
#include <QVariantList>
#include <QList>
#include <QString>
#include <QByteArray>
#include <QFile>
#include "blockslistmodel.h"

// Seed
BlocksListModel::BlocksListModel(QObject* parent) : QAbstractListModel(parent) {
    m_blocks.push_back(Document{"100.0342.0","Inconel718","10","2000"});
    m_blocks.push_back(
        Block{
            "1.1",
            "80MN",
            "Custom",
            "Top 650",
            "Bottom 650",
            "<--",
            "200",
            "130",
            "200",
            "20",
            "40",
            "40"
        }
    );
    m_blocks.push_back(Heat{"1.2", "min", "0, 1000, 350, 1000"});
    m_blocks.push_back(Upset{ "(1000)->800->700" });
    m_blocks.push_back(Draw{ "(600)->550->(90)580->(90)535->(90)560->(45)600->(45)610" });
    m_blocks.push_back(
    Block{
        "2.1",
        "45MN",
        "Custom",
        "Top 550",
        "Bottom 550",
        "<--",
        "180",
        "120",
        "180",
        "30",
        "50",
        "50"
        }
    );
    m_blocks.push_back(Heat{"2.2", "min", "0, 980, 250, 980"});
    m_blocks.push_back(Draw{ "(500)->450->(90)480->(90)435->(90)460->(45)500->(45)510" });
    m_blocks.push_back(Upset{ "(1400)->1100->900" });
}

int BlocksListModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : m_types.size();
}

QVariant BlocksListModel::data(const QModelIndex &index, const int role) const {
    if (!checkIndex(index, CheckIndexOption::IndexIsValid))
        return QVariant();

    if (!index.isValid())
        return QVariant();

    const int row = index.row();

    switch (role) {
        case TypeRole:     return m_types[row];
        case ContentRole:  return m_blocks[row];
    }
    return QVariant();
}

QHash<int, QByteArray> BlocksListModel::roleNames() const {
    QHash<int, QByteArray> mapping {
        {Roles::TypeRole, "blockType"},
        {Roles::ContentRole, "blockContent"},
    };
    return mapping;
}

Qt::ItemFlags BlocksListModel::flags(const QModelIndex &index) const {
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

bool BlocksListModel::setData(const QModelIndex &index, const QVariant &value, int role) {

    // if (!index.isValid()) return false;
    if (role != Qt::EditRole) return false;

    const int row = index.row();
    if (row < 0 || row >= m_types.size()) return false;
    if (!value.canConvert<QVariantMap>()) return false;

    const QVariantMap payload = value.toMap();

    // Expect a composite object: { "type": ..., "content": ... }
    if (!payload.contains("type")) return false;
    if (!payload.contains("content")) return false;

    m_types[row] = std::move(payload.value("type").toString());
    m_blocks[row] = std::move(payload.value("content").toMap());

    emit dataChanged(index, index);

    // Similarly: after insert/remove/moveRows or clear, call updateDocumentBeginCache().
    updateDocumentCache();

    return true;
}

bool BlocksListModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationRow) {
    int newDestination = destinationRow;
    if (sourceRow < destinationRow)
        newDestination++;

    beginMoveRows(sourceParent, sourceRow, sourceRow, destinationParent, newDestination);

    m_types.move(sourceRow, destinationRow);
    m_blocks.move(sourceRow, destinationRow);

    endMoveRows();

    updateDocumentCache();

    return true;
}

bool BlocksListModel::insertRows(int row, int count, const QModelIndex &parent) {
    beginInsertRows(parent, row, row + count - 1);

    for (int i = 0; i < count; ++i) {
        m_types.insert(row, QString{});
        m_blocks.insert(row, QVariantMap{});
    }

    endInsertRows();

    // Similarly: after insert/remove/moveRows or clear, call updateDocumentBeginCache().
    updateDocumentCache();

    return true;
}

bool BlocksListModel::removeRows(int row, int count, const QModelIndex &parent) {
    beginRemoveRows(parent, row, row + count - 1);

    for (int i = 0; i < count; ++i) {
        m_types.removeAt(row);
        m_blocks.removeAt(row);
    }

    endRemoveRows();

    // Similarly: after insert/remove/moveRows or clear, call updateDocumentBeginCache().
    updateDocumentCache();

    return true;
}

bool BlocksListModel::appendRow(int rowNumber) {
    beginInsertRows(QModelIndex(), rowNumber, rowNumber);

    m_types.append(QString{});
    m_blocks.append(QVariantMap{});

    endInsertRows();

    updateDocumentCache();

    return true;
}

bool BlocksListModel::clearModel(int rowNumber) {
    const int previousRow = rowNumber - 1;
    beginRemoveRows(QModelIndex(), 0, previousRow);

    m_types.remove(0, previousRow);
    m_blocks.remove(0, previousRow);

    endRemoveRows();

    updateDocumentCache();

    return true;
}
