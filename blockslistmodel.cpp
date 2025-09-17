#include <QJsonObject>
#include <QJsonDocument>
#include <QVariantMap>
#include <QVariantList>
#include <QList>
#include <QString>
#include <QByteArray>
#include "blockslistmodel.h"

BlocksListModel::BlocksListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    blocksTypes = {"documentBegin", "blockBegin", "heat", "upset", "draw", "blockBegin", "heat", "draw", "upset"};

    blocksContents = {
        QVariantMap{
            {"documentNumber", "100.0342.0"},
            {"material", "Inconel718"},
            {"meshDensity", 10}
        },
        QVariantMap{
            {"press", "80MN"},
            {"dies", "V-dies 320-260"},
            {"feedFirst", 250},
            {"feedOther", 200},
            {"speedUpset", 20},
            {"speedDraw", 80}
        },
        QVariantMap{
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
            {"operations", "(1000)->800->700"}
        },
        QVariantMap{
            {"operations", "(600)->550->(90)580->(90)535->(90)560->(45)600->(45)610"}
        },
        QVariantMap{
            {"press", "40MN"},
            {"dies", "V-dies 280-240"},
            {"feedFirst", 200},
            {"feedOther", 100},
            {"speedUpset", 20},
            {"speedDraw", 80}
        },
        QVariantMap{
            {"operations", "(500)->450->(90)480->(90)435->(90)460->(45)500->(45)510"}
        },
        QVariantMap{
            {"operations", "(1400)->1100->900"}
        },
    };
}

int BlocksListModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return blocksTypes.size();
}

QVariant BlocksListModel::data(const QModelIndex &index, int role) const {
    if (!checkIndex(index, CheckIndexOption::IndexIsValid))
        return QVariant();

    if (!index.isValid())
        return QVariant();

    const int row = index.row();

    switch (role) {
        case BlockType:     return blocksTypes[row];
        case BlockContent:  return blocksContents[row];
    }
    return QVariant();
}

QHash<int, QByteArray> BlocksListModel::roleNames() const {
    QHash<int, QByteArray> mapping {
        {BlocksRoles::BlockType, "blockType"},
        {BlocksRoles::BlockContent, "blockContent"},
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
    if (row < 0 || row >= blocksTypes.size()) return false;
    if (!value.canConvert<QVariantMap>()) return false;

    const QVariantMap payload = value.toMap();

    // Expect a composite object: { "type": ..., "content": ... }
    if (!payload.contains("type")) return false;
    if (!payload.contains("content")) return false;

    blocksTypes[row] = std::move(payload.value("type").toString());
    blocksContents[row] = std::move(payload.value("content").toMap());

    emit dataChanged(index, index);
    return true;
}

bool BlocksListModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationRow) {
    int newDestination = destinationRow;
    if (sourceRow < destinationRow)
        newDestination++;

    beginMoveRows(sourceParent, sourceRow, sourceRow, destinationParent, newDestination);

    blocksTypes.move(sourceRow, destinationRow);
    blocksContents.move(sourceRow, destinationRow);

    endMoveRows();
    return true;
}

bool BlocksListModel::insertRows(int row, int count, const QModelIndex &parent) {
    beginInsertRows(parent, row, row + count - 1);

    for (int i = 0; i < count; ++i)
        blocksTypes.insert(row, QString{});
        blocksContents.insert(row, QVariantMap{});

    endInsertRows();
    return true;
}

bool BlocksListModel::removeRows(int row, int count, const QModelIndex &parent) {
    beginRemoveRows(parent, row, row + count - 1);

    for (int i = 0; i < count; ++i)
        blocksTypes.removeAt(row);
        blocksContents.removeAt(row);

    endRemoveRows();
    return true;
}

bool BlocksListModel::appendRow(int rowNumber) {
    beginInsertRows(QModelIndex(), rowNumber, rowNumber);

    blocksTypes.append(QString{});
    blocksContents.append(QVariantMap{});

    endInsertRows();
    return true;
}

bool BlocksListModel::clearModel(int rowNumber) {
    const int previousRow = rowNumber - 1;
    beginRemoveRows(QModelIndex(), 0, previousRow);

    blocksTypes.remove(0, previousRow);
    blocksContents.remove(0, previousRow);

    endRemoveRows();
    return true;
}
