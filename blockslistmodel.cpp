#include <QJsonObject>
#include <QJsonDocument>
#include <QVariantMap>
#include <QVariantList>
#include <QList>
#include <QString>
#include <QByteArray>
#include "blockslistmodel.h"
// #include "BaseItem.h"

BlocksListModel::BlocksListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_types = {"document", "block", "heat", "upset", "draw", "block", "heat", "draw", "upset"};

    m_blocks = {
        QVariantMap{
            {"type", "document"},
            {"image", 1},
            {"documentNumber", "100.0342.0"},
            {"material", "Inconel718"},
            {"meshDensity", 10}
        },
        QVariantMap{
            {"type", "block"},
            {"image", 2},
            {"press", "80MN"},
            {"dies", "V-dies 320-260"},
            {"feedFirst", 250},
            {"feedOther", 200},
            {"speedUpset", 20},
            {"speedDraw", 80}
        },
        QVariantMap{
            {"type", "heat"},
            {"image", 3},
            {"timeUnits", "minutes"},
            {"typeTimeTemperature", QVariantList{
                    "preheatFurnace", 0, 1000,
                    "openDoor", 20, 1000,
                    "openDoor", 40, 700,
                    "heating", 60, 1000,
                    "heating", 360, 1000,
                    "soaking", 400, 1000
                }
            }
        },
        QVariantMap{
            {"type", "upset"},
            {"image", 4},
            {"operations", "(1000)->800->700"}
        },
        QVariantMap{
            {"type", "draw"},
            {"image", 5},
            {"operations", "(600)->550->(90)580->(90)535->(90)560->(45)600->(45)610"}
        },
        QVariantMap{
            {"type", "block"},
            {"image", 6},
            {"press", "40MN"},
            {"dies", "V-dies 280-240"},
            {"feedFirst", 200},
            {"feedOther", 100},
            {"speedUpset", 20},
            {"speedDraw", 80}
        },
        QVariantMap{
            {"type", "heat"},
            {"image", 7},
            {"timeUnits", "minutes"},
            {"typeTimeTemperature", QVariantList{
                                           "preheatFurnace", 0, 1000,
                                           "openDoor", 20, 1000,
                                           "openDoor", 40, 700,
                                           "heating", 60, 1000,
                                           "heating", 360, 1000,
                                           "soaking", 400, 1000
                                       }
            }
        },
        QVariantMap{
            {"type", "draw"},
            {"image", 8},
            {"operations", "(500)->450->(90)480->(90)435->(90)460->(45)500->(45)510"}
        },
        QVariantMap{
            {"type", "upset"},
            {"image", 9},
            {"operations", "(1400)->1100->900"}
        },
    };
}

int BlocksListModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return m_types.size();
}

QVariant BlocksListModel::data(const QModelIndex &index, int role) const {
    if (!checkIndex(index, CheckIndexOption::IndexIsValid))
        return QVariant();

    if (!index.isValid())
        return QVariant();

    const int row = index.row();

    switch (role) {
        case BlockTypeRole:     return m_types[row];
        case BlockContentRole:  return m_blocks[row];
    }
    return QVariant();
}

QHash<int, QByteArray> BlocksListModel::roleNames() const {
    QHash<int, QByteArray> mapping {
        {BlocksRoles::BlockTypeRole, "blockType"},
        {BlocksRoles::BlockContentRole, "blockContent"},
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
