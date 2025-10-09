#include <QJsonObject>
// #include <QJsonDocument>
#include <QVariantMap>
// #include <QVariantList>
#include <QList>
#include <QString>
#include <QByteArray>
#include "blockslistmodel.h"
#include "BaseItem.h"

BlocksListModel::BlocksListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_types = {"documentBegin", "blockBegin", "heat", "upset", "draw", "blockBegin", "heat", "draw", "upset"};

    // Prepare shared pointers that could be shared among items
    // auto docNumber = std::make_shared<std::string>("100.0342.0");
    // auto curve = std::make_shared<std::vector<double>>();
    // std::shared_ptr<std::string> currentBlockNamePtr; // will be shared among items in the same block

    {
        auto it = std::make_unique<DocBeginItem>();
        // QVariantMap p;
        // p.insert("type", "documentBegin");
        // p.insert("image", 1);
        // p.insert("documentNumber", QString::fromStdString(*docNumber));
        // p.insert("material", "Inconel718");
        // p.insert("meshDensity", 10);
        // it->setDocumentNumberPtr(docNumber);
        // it->setBlockNamePtr(currentBlockNamePtr);
        // it->setPayload(p);
        m_blocks.append(std::move(it));
    }
    {
        auto it = std::make_unique<BlockBeginItem>();
        // it->setBlockNamePtr(currentBlockNamePtr);
        // QVariantMap p;
        // p.insert("type", "blockBegin");
        // p.insert("image", 2);
        // p.insert("press", "80MN");
        // p.insert("dies", "V-dies 320-260");
        // p.insert("feedFirst", 250);
        // p.insert("feedOther", 200);
        // p.insert("speedUpset", 20);
        // p.insert("speedDraw", 80);
        // it->setPayload(p);
        m_blocks.append(std::move(it));
        // currentBlockNamePtr = m_blocks.back()->blockNamePtr();
    }
    {
        auto it = std::make_unique<HeatItem>();
        // it->setBlockNamePtr(currentBlockNamePtr);
        // QVariantMap p;
        // p.insert("type", "heat");
        // p.insert("image", 3);
        // p.insert("timeUnits", "minutes");
        // p.insert("typeTimeTemperature", QVariantList{
        //     "preheatFurnace", 0, 1000,
        //     "openDoor", 20, 1000,
        //     "openDoor", 40, 700,
        //     "heating", 60, 1000,
        //     "heating", 360, 1000,
        //     "soaking", 400, 1000
        // });
        // it->setPayload(p);
        m_blocks.append(std::move(it));
    }
    {
        auto it = std::make_unique<BaseItem>();
        // it->setBlockNamePtr(currentBlockNamePtr);
        // QVariantMap p;
        // p.insert("type", "upset");
        // p.insert("image", 4);
        // p.insert("operations", "(1000)->800->700");
        // it->setPayload(p);
        m_blocks.append(std::move(it));
    }
    {
        auto it = std::make_unique<BaseItem>();
        // it->setBlockNamePtr(currentBlockNamePtr);
        // QVariantMap p;
        // p.insert("type", "draw");
        // p.insert("image", 5);
        // p.insert("operations", "(600)->550->(90)580->(90)535->(90)560->(45)600->(45)610");
        // it->setPayload(p);
        m_blocks.append(std::move(it));
    }
    {
        auto it = std::make_unique<BlockBeginItem>();
        // it->setBlockNamePtr(currentBlockNamePtr);
        // QVariantMap p;
        // p.insert("type", "blockBegin");
        // p.insert("image", 6);
        // p.insert("press", "40MN");
        // p.insert("dies", "V-dies 280-240");
        // p.insert("feedFirst", 200);
        // p.insert("feedOther", 100);
        // p.insert("speedUpset", 20);
        // p.insert("speedDraw", 80);
        // it->setPayload(p);
        m_blocks.append(std::move(it));
        // currentBlockNamePtr = m_blocks.back()->blockNamePtr();
    }
    {
        auto it = std::make_unique<HeatItem>();
        // it->setBlockNamePtr(currentBlockNamePtr);
        // QVariantMap p;
        // p.insert("type", "heat");
        // p.insert("image", 7);
        // p.insert("timeUnits", "minutes");
        // p.insert("typeTimeTemperature", QVariantList{
        //     "preheatFurnace", 0, 1000,
        //     "openDoor", 20, 1000,
        //     "openDoor", 40, 700,
        //     "heating", 60, 1000,
        //     "heating", 360, 1000,
        //     "soaking", 400, 1000
        // });
        // it->setPayload(p);
        m_blocks.append(std::move(it));
    }
    {
        auto it = std::make_unique<BaseItem>();
        // it->setBlockNamePtr(currentBlockNamePtr);
        // QVariantMap p;
        // p.insert("type", "draw");
        // p.insert("image", 8);
        // p.insert("operations", "(500)->450->(90)480->(90)435->(90)460->(45)500->(45)510");
        // it->setPayload(p);
        m_blocks.append(std::move(it));
    }
    {
        auto it = std::make_unique<BaseItem>();
        // it->setBlockNamePtr(currentBlockNamePtr);
        // QVariantMap p;
        // p.insert("type", "upset");
        // p.insert("image", 9);
        // p.insert("operations", "(1400)->1100->900");
        // it->setPayload(p);
        m_blocks.append(std::move(it));
    }
}

int BlocksListModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return static_cast<int>(m_types.size());
}

QVariant BlocksListModel::data(const QModelIndex &index, int role) const {
    if (!checkIndex(index, CheckIndexOption::IndexIsValid))
        return QVariant();

    if (!index.isValid())
        return QVariant();

    const int row = index.row();

    switch (role) {
        case TypeRole:     return m_types[row];
        // case ContentRole:  return m_blocks[row]->toVariantMap();
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
    {
        auto map = payload.value("content").toMap();
        // m_blocks[row]->setPayload(map);
    }

    emit dataChanged(index, index);

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
        auto newItem = std::make_unique<BaseItem>();
        if (row > 0) {
            // newItem->setBlockNamePtr(m_blocks[row - 1]->blockNamePtr());
        }
        m_blocks.insert(row, std::move(newItem));
        if (row + 1 < m_blocks.size()) {
            // m_blocks[row + 1]->setBlockNamePtr(m_blocks[row]->blockNamePtr());
        }
        ++row;
    }

    endInsertRows();

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

    updateDocumentCache();

    return true;
}

bool BlocksListModel::appendRow(int rowNumber) {
    beginInsertRows(QModelIndex(), rowNumber, rowNumber);

    m_types.append(QString{});

    auto newItem = std::make_unique<BaseItem>();
    if (!m_blocks.isEmpty()) {
        // newItem->setBlockNamePtr(m_blocks.back()->blockNamePtr());
    }
    m_blocks.append(std::move(newItem));

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
