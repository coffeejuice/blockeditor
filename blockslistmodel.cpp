#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QString>
#include <QByteArray>
#include "blockslistmodel.h"

static QJsonObject parseObject(const QString &json)
{
    return QJsonDocument::fromJson(json.toUtf8()).object();
}

BlocksListModel::BlocksListModel(QObject *parent) : QAbstractListModel{parent} {
    // blocksTypes = {"documentBegin", "blockBegin", "heat", "upset", "draw", "blockBegin", "heat", "draw", "upset"};
    blocksContents = {
        parseObject(R"({
            "type":"documentBegin",
            "content":{"documentNumber":"100.0342.0","material":"Inconel718","meshDensity":10}})"),
        parseObject(R"({
            "type":"beginBlock",
            "content":{"press":"80MN","dies":"V-dies 320-260","feedFirst":250,"feedOther":200,"speedUpset":20,"speedDraw":80}})"),
        parseObject(R"({
            "type":"heat",
            "content":{"timeUnits":"minutes","typeTimeTemperature":"preheatFurnace, 0, 1000, openDoor, 20, 1000, openDoor, 40, 700, heating, 60, 1000, heating, 360, 1000, soaking, 400, 1000"}})"),
        parseObject(R"({
            "type":"upset",
            "content":"(1000)->800->700")"),
        parseObject(R"({
            "type":"draw",
            "content":"(600)->550->(90)580->(90)535->(90)560->(45)600->(45)610")"),
        parseObject(R"({
            "type":"blockBegin",
            "content":{"press":"40MN","dies":"V-dies 280-240","feedFirst":200,"feedOther":100,"speedUpset":20,"speedDraw":80}})"),
        parseObject(R"({
            "type":"draw",
            "content":"(500)->450->(90)480->(90)435->(90)460->(45)500->(45)510")"),
        parseObject(R"({
            "type":"upset",
            "content":"(1400)->1100->900")"),
    };
}

int BlocksListModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return blocksContents.length();
}

QVariant BlocksListModel::data(const QModelIndex &index, int role) const {
    if (!checkIndex(index, CheckIndexOption::IndexIsValid))
        return QVariant();

    if (!index.isValid())
        return QVariant();

    const int row = index.row();

    switch (role) {
    // case BlockType:
    //     return blocksTypes[row];
    case BlockContent:
        return blocksContents[row];
    }
    return QVariant();
}

QHash<int, QByteArray> BlocksListModel::roleNames() const {
    QHash<int, QByteArray> mapping {
        // {BlocksRoles::BlockType, "blockType"},
        {BlocksRoles::BlockContent, "blockContent"},
    };
    return mapping;
}

Qt::ItemFlags BlocksListModel::flags(const QModelIndex &index) const {
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

bool BlocksListModel::setData(const QModelIndex &index, const QVariant &value, int role) {

    if (role != Qt::EditRole || !index.isValid() || value.canConvert<QJsonObject>())
        return false;

    const int row = index.row();

    const QJsonObject obj = value.value<QJsonObject>();
    // blocksTypes[row] = obj.value("type").toString();
    // blocksContents[row] = obj.value("content").toObject();  // store as QJsonObject
    blocksContents[row] = obj;  // store as QJsonObject
    emit dataChanged(index, index);
    return true;
}

bool BlocksListModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationRow) {
    int newDestination = destinationRow;
    if (sourceRow < destinationRow) {
        newDestination++;
    }

    beginMoveRows(sourceParent, sourceRow, sourceRow, destinationParent, newDestination);
    blocksContents.move(sourceRow, destinationRow);
    endMoveRows();

    return true;
}

bool BlocksListModel::insertRows(int row, int count, const QModelIndex &parent) {
    beginInsertRows(parent, row, row + count - 1);
    blocksContents.insert(row, QJsonObject());
    endInsertRows();
    return true;
}

bool BlocksListModel::removeRows(int row, int count, const QModelIndex &parent) {
    beginRemoveRows(parent, row, row + count - 1);
    blocksContents.removeAt(row);
    endRemoveRows();
    return true;
}

bool BlocksListModel::appendRow(int rowNumber) {
    beginInsertRows(QModelIndex(), rowNumber, rowNumber);
    blocksContents.append(QJsonObject());
    endInsertRows();
    return true;
}

bool BlocksListModel::clearModel(int rowNumber) {
    beginRemoveRows(QModelIndex(), 0, rowNumber - 1);
    blocksContents.remove(0, rowNumber - 1);
    endRemoveRows();
    return true;
}
