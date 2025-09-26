#include <QJsonObject>
#include <QJsonDocument>
#include <QVariantMap>
#include <QVariantList>
#include <QList>
#include <QString>
#include <QByteArray>
#include "blockslistmodel.h"
#include "BaseItem.h"

BlocksListModel::BlocksListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_blocksTypes = {"documentBegin", "blockBegin", "heat", "upset", "draw", "blockBegin", "heat", "draw", "upset"};

    // Prepare shared pointers that could be shared among items
    auto docNumber = std::make_shared<std::string>("100.0342.0");
    auto curve = std::make_shared<std::vector<double>>();
    std::shared_ptr<std::string> currentBlockNamePtr; // will be shared among items in the same block

    // Build items with payloads similar to the original QVariantMaps
    {
        DocBeginItem it;
        QVariantMap p;
        p.insert("type", "documentBegin");
        p.insert("image", 1);
        p.insert("documentNumber", QString::fromStdString(*docNumber));
        p.insert("material", "Inconel718");
        p.insert("meshDensity", 10);
        it.setDocumentNumberPtr(docNumber);
        // propagate current block name pointer (may be null at the beginning)
        it.setBlockNamePtr(currentBlockNamePtr);
        it.setPayload(p);
        m_blocksContents.append(it);
    }
    {
        BlockBeginItem it;
        // as per requirement, new BlockBeginItem should point to previous item's block name pointer
        it.setBlockNamePtr(currentBlockNamePtr);
        QVariantMap p;
        p.insert("type", "blockBegin");
        p.insert("image", 2);
        p.insert("press", "80MN");
        p.insert("dies", "V-dies 320-260");
        p.insert("feedFirst", 250);
        p.insert("feedOther", 200);
        p.insert("speedUpset", 20);
        p.insert("speedDraw", 80);
        it.setPayload(p);
        m_blocksContents.append(it);
        // next items should point to the newly inserted item's pointer
        currentBlockNamePtr = m_blocksContents.back().blockNamePtr();
    }
    {
        HeatItem it;
        it.setBlockNamePtr(currentBlockNamePtr);
        QVariantMap p;
        p.insert("type", "heat");
        p.insert("image", 3);
        p.insert("timeUnits", "minutes");
        p.insert("typeTimeTemperature", QVariantList{
            "preheatFurnace", 0, 1000,
            "openDoor", 20, 1000,
            "openDoor", 40, 700,
            "heating", 60, 1000,
            "heating", 360, 1000,
            "soaking", 400, 1000
        });
        it.setPayload(p);
        m_blocksContents.append(it);
    }
    {
        BaseItem it;
        it.setBlockNamePtr(currentBlockNamePtr);
        QVariantMap p;
        p.insert("type", "upset");
        p.insert("image", 4);
        p.insert("operations", "(1000)->800->700");
        it.setPayload(p);
        m_blocksContents.append(it);
    }
    {
        BaseItem it;
        it.setBlockNamePtr(currentBlockNamePtr);
        QVariantMap p;
        p.insert("type", "draw");
        p.insert("image", 5);
        p.insert("operations", "(600)->550->(90)580->(90)535->(90)560->(45)600->(45)610");
        it.setPayload(p);
        m_blocksContents.append(it);
    }
    {
        BlockBeginItem it;
        // new BlockBeginItem should share previous pointer
        it.setBlockNamePtr(currentBlockNamePtr);
        QVariantMap p;
        p.insert("type", "blockBegin");
        p.insert("image", 6);
        p.insert("press", "40MN");
        p.insert("dies", "V-dies 280-240");
        p.insert("feedFirst", 200);
        p.insert("feedOther", 100);
        p.insert("speedUpset", 20);
        p.insert("speedDraw", 80);
        it.setPayload(p);
        m_blocksContents.append(it);
        // subsequent items point to this new one
        currentBlockNamePtr = m_blocksContents.back().blockNamePtr();
    }
    {
        HeatItem it;
        it.setBlockNamePtr(currentBlockNamePtr);
        QVariantMap p;
        p.insert("type", "heat");
        p.insert("image", 7);
        p.insert("timeUnits", "minutes");
        p.insert("typeTimeTemperature", QVariantList{
            "preheatFurnace", 0, 1000,
            "openDoor", 20, 1000,
            "openDoor", 40, 700,
            "heating", 60, 1000,
            "heating", 360, 1000,
            "soaking", 400, 1000
        });
        it.setPayload(p);
        m_blocksContents.append(it);
    }
    {
        BaseItem it;
        it.setBlockNamePtr(currentBlockNamePtr);
        QVariantMap p;
        p.insert("type", "draw");
        p.insert("image", 8);
        p.insert("operations", "(500)->450->(90)480->(90)435->(90)460->(45)500->(45)510");
        it.setPayload(p);
        m_blocksContents.append(it);
    }
    {
        BaseItem it;
        it.setBlockNamePtr(currentBlockNamePtr);
        QVariantMap p;
        p.insert("type", "upset");
        p.insert("image", 9);
        p.insert("operations", "(1400)->1100->900");
        it.setPayload(p);
        m_blocksContents.append(it);
    }
}

int BlocksListModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return m_blocksTypes.size();
}

QVariant BlocksListModel::data(const QModelIndex &index, int role) const {
    if (!checkIndex(index, CheckIndexOption::IndexIsValid))
        return QVariant();

    if (!index.isValid())
        return QVariant();

    const int row = index.row();

    switch (role) {
        case BlockTypeRole:     return m_blocksTypes[row];
        case BlockContentRole:  return m_blocksContents[row].toVariantMap();
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
    if (row < 0 || row >= m_blocksTypes.size()) return false;
    if (!value.canConvert<QVariantMap>()) return false;

    const QVariantMap payload = value.toMap();

    // Expect a composite object: { "type": ..., "content": ... }
    if (!payload.contains("type")) return false;
    if (!payload.contains("content")) return false;

    m_blocksTypes[row] = std::move(payload.value("type").toString());
    {
        auto map = payload.value("content").toMap();
        m_blocksContents[row].setPayload(map);
    }

    emit dataChanged(index, index);

    // Similarly: after insert/remove/moveRows or clear, call updateDocumentBeginCache().
    updateDocumentBeginCache();

    return true;
}

bool BlocksListModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationRow) {
    int newDestination = destinationRow;
    if (sourceRow < destinationRow)
        newDestination++;

    beginMoveRows(sourceParent, sourceRow, sourceRow, destinationParent, newDestination);

    m_blocksTypes.move(sourceRow, destinationRow);
    m_blocksContents.move(sourceRow, destinationRow);

    endMoveRows();

    updateDocumentBeginCache();

    return true;
}

bool BlocksListModel::insertRows(int row, int count, const QModelIndex &parent) {
    beginInsertRows(parent, row, row + count - 1);

    for (int i = 0; i < count; ++i) {
        m_blocksTypes.insert(row, QString{});
        BaseItem newItem;
        // Link the new item's block name pointer to the previous item's pointer (if any)
        if (row > 0) {
            newItem.setBlockNamePtr(m_blocksContents[row - 1].blockNamePtr());
        }
        m_blocksContents.insert(row, newItem);
        // Ensure the next item's pointer (if exists) points to the newly inserted item's pointer
        if (row + 1 < m_blocksContents.size()) {
            m_blocksContents[row + 1].setBlockNamePtr(m_blocksContents[row].blockNamePtr());
        }
        // advance row for multiple inserts so subsequent insert positions move forward
        ++row;
    }

    endInsertRows();

    // Similarly: after insert/remove/moveRows or clear, call updateDocumentBeginCache().
    updateDocumentBeginCache();

    return true;
}

bool BlocksListModel::removeRows(int row, int count, const QModelIndex &parent) {
    beginRemoveRows(parent, row, row + count - 1);

    for (int i = 0; i < count; ++i) {
        m_blocksTypes.removeAt(row);
        m_blocksContents.removeAt(row);
    }

    endRemoveRows();

    // Similarly: after insert/remove/moveRows or clear, call updateDocumentBeginCache().
    updateDocumentBeginCache();

    return true;
}

bool BlocksListModel::appendRow(int rowNumber) {
    beginInsertRows(QModelIndex(), rowNumber, rowNumber);

    m_blocksTypes.append(QString{});

    BaseItem newItem;
    if (!m_blocksContents.isEmpty()) {
        newItem.setBlockNamePtr(m_blocksContents.back().blockNamePtr());
    }
    m_blocksContents.append(newItem);

    endInsertRows();

    updateDocumentBeginCache();

    return true;
}

bool BlocksListModel::clearModel(int rowNumber) {
    const int previousRow = rowNumber - 1;
    beginRemoveRows(QModelIndex(), 0, previousRow);

    m_blocksTypes.remove(0, previousRow);
    m_blocksContents.remove(0, previousRow);

    endRemoveRows();

    updateDocumentBeginCache();

    return true;
}
