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
    appendDocument("100.0342.0","Inconel718","10","2000");
    appendBlock(
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
    );
    appendHeat("1.2", "min", "0, 1000, 350, 1000");
    appendUpset("(1000)->800->700");
    appendDraw("(600)->550->(90)580->(90)535->(90)460->(45)500->(45)510");
    appendBlock(
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
    );
    appendHeat("2.2", "min", "0, 980, 250, 980");
    appendDraw("(500)->450->(90)480->(90)435->(90)460->(45)500->(45)510");
    appendUpset("(1400)->1100->900");
}

int BlocksListModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : static_cast<int>(m_blocks.size());
}

// Small helpers that defer everything to the active struct
static QString typeOf(const ItemVariant& v) {
    return std::visit([](auto const& obj)->QString { return obj.type(); }, v);
}

static int fieldCountOf(const ItemVariant& v) {
    return std::visit([](auto const& obj)->int { return obj.fieldCount(); }, v);
}

static QString getSlot(const ItemVariant& v, int slot) {
    return std::visit([&](auto const& obj)->QString { return obj.get(slot); }, v);
}

static bool setSlot(ItemVariant& v, int slot, const QString& value) {
    return std::visit([&](auto& obj)->bool { return obj.set(slot, value); }, v);
}

QVariant BlocksListModel::data(const QModelIndex &index, const int role) const {

    if (!checkIndex(index, CheckIndexOption::IndexIsValid)) return {};

    const auto& v = m_blocks[index.row()];

    if (role == TypeRole)           return typeOf(v);
    if (role == FieldCountRole)     return fieldCountOf(v);

    // slot data
    if (const int slot = slotFromRole(role); slot >= 0)     return getSlot(v, slot);

    return {};
}

bool BlocksListModel::setData(const QModelIndex &index, const QVariant &value, int role) {

    if (!checkIndex(index, CheckIndexOption::IndexIsValid)) return false;
    // if (role != Qt::EditRole) return false;

    const int slot = slotFromRole(role);
    if (slot < 0) return false;

    auto& v = m_blocks[index.row()];
    // Keep edits within the active type's slot range
    if (slot >= fieldCountOf(v)) return false;
    if (!setSlot(v, slot, value.toString())) return false; // Not changed

    emit dataChanged(index, index, { role });

    // Similarly: after insert/remove/moveRows or clear, call updateDocumentBeginCache().
    // updateDocumentCache();

    return true;
}

Qt::ItemFlags BlocksListModel::flags(const QModelIndex &index) const {
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> BlocksListModel::roleNames() const {

    QHash<int, QByteArray> mapping {
            { TypeRole,       "type" },
            { ARole,          "a" },
            { BRole,          "b" },
            { CRole,          "c" },
            { DRole,          "d" },
            { ERole,          "e" },
            { FRole,          "f" },
            { GRole,          "g" },
            { HRole,          "h" },
            { IRole,          "i" },
            { JRole,          "j" },
            { KRole,          "k" },
            { LRole,          "l" },
            { FieldCountRole, "fieldCount" }
    };
    return mapping;
}
// QML helpers
void BlocksListModel::appendDocument(const QString& name, const QString& material_id, const QString& mesh_elements, const QString& weight) {
    const int r = static_cast<int>(m_blocks.size());
    beginInsertRows({}, r, r);
    Document d;
    d.name = name;
    d.material_id = material_id;
    d.mesh_elements = mesh_elements;
    d.weight = weight;
    m_blocks.push_back(d);
    endInsertRows();
}
void BlocksListModel::appendBlock(
        const QString& name,
        const QString& press_id,
        const QString& die_assembly_id,
        const QString& top_die_id,
        const QString& bottom_die_id,
        const QString& feed_direction_id,
        const QString& feed_first,
        const QString& feed_middle,
        const QString& feed_last,
        const QString& speed_upsetting,
        const QString& speed_prolongation,
        const QString& speed_full_die
) {
    const int r = static_cast<int>(m_blocks.size());
    beginInsertRows({}, r, r);
    Block b;
    b.name = name;
    b.press_id = press_id;
    b.die_assembly_id = die_assembly_id;
    b.top_die_id = top_die_id;
    b.bottom_die_id = bottom_die_id;
    b.feed_direction_id = feed_direction_id;
    b.feed_first = feed_first;
    b.feed_middle = feed_middle;
    b.feed_last = feed_last;
    b.speed_upsetting = speed_upsetting;
    b.speed_prolongation = speed_prolongation;
    b.speed_full_die = speed_full_die;
    m_blocks.push_back(b);
    endInsertRows();
}
void BlocksListModel::appendHeat(const QString& name, const QString& timeUnits, const QString& typeTimeTemperature) {
    const int r = static_cast<int>(m_blocks.size());
    beginInsertRows({}, r, r);
    Heat h;
    h.name = name;
    h.timeUnits = timeUnits;
    h.typeTimeTemperature = typeTimeTemperature;
    m_blocks.push_back(h);
    endInsertRows();
}
void BlocksListModel::appendUpset(const QString& operations) {
    const int r = static_cast<int>(m_blocks.size());
    beginInsertRows({}, r, r);
    Upset u;
    u.operations = operations;
    m_blocks.push_back(u);
    endInsertRows();
}
void BlocksListModel::appendDraw(const QString& operations) {
    const int r = static_cast<int>(m_blocks.size());
    beginInsertRows({}, r, r);
    Draw d;
    d.operations = operations;
    m_blocks.push_back(d);
    endInsertRows();
}

// bool BlocksListModel::appendRow(const int rowNumber) {
//     beginInsertRows(QModelIndex(), rowNumber, rowNumber);
//
//     m_types.append(QString{});
//
//     endInsertRows();
//
//     updateDocumentCache();
//
//     return true;
// }

void BlocksListModel::removeRowAt(const int row) {
    if (row < 0 || row >= m_blocks.size()) return;
    beginRemoveRows({}, row, row);
    m_blocks.removeAt(row);
    endRemoveRows();
    updateDocumentCache();
}

bool BlocksListModel::saveToFile(const QString& filePath) const {
    QJsonArray arr;
    for (const auto& v : m_blocks) {
        QJsonObject o;
        o["type"] = typeOf(v);
        // per-type JSON emitted by each struct
        {
            const QJsonObject per = std::visit([](auto const& obj){ return obj.toJson(); }, v);
            for (auto it = per.begin(); it != per.end(); ++it) {
                o.insert(it.key(), it.value());
            }
        }
        arr.append(o);
    }
    QFile f(filePath);
    if (!f.open(QIODevice::WriteOnly)) return false;
    f.write(QJsonDocument(arr).toJson(QJsonDocument::Indented));
    return true;
}

bool BlocksListModel::loadFromFile(const QString& filePath) {
    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly)) return false;
    const auto doc = QJsonDocument::fromJson(f.readAll());
    if (!doc.isArray()) return false;

    const auto arr = doc.array();
    beginResetModel();
    m_blocks.clear();
    m_blocks.reserve(arr.size());
    for (const auto& v : arr) {
        const auto o = v.toObject();
        const auto t = o["type"].toString();
        if (t == Document::type())   { Document d; d.fromJson(o); m_blocks.push_back(d); }
        else if (t == Block::type()) { Block b; b.fromJson(o); m_blocks.push_back(b); }
        else if (t == Heat::type())  { Heat h; h.fromJson(o); m_blocks.push_back(h); }
        else if (t == Upset::type()) { Upset u; u.fromJson(o); m_blocks.push_back(u); }
        else if (t == Draw::type())  { Draw d; d.fromJson(o); m_blocks.push_back(d); }
        else throw std::runtime_error(("Unknown block type: " + t).toStdString());
    }
    endResetModel();
    return true;
}

bool BlocksListModel::setField(const int row, const QString& roleName, const QVariant& value) {
    int role = -1;
    if (roleName == "a") role = ARole;
    else if (roleName == "b") role = BRole;
    else if (roleName == "c") role = CRole;
    else if (roleName == "d") role = DRole;
    else if (roleName == "e") role = ERole;
    else if (roleName == "f") role = FRole;
    else if (roleName == "g") role = GRole;
    else if (roleName == "h") role = HRole;
    else if (roleName == "i") role = IRole;
    else if (roleName == "j") role = JRole;
    else if (roleName == "k") role = KRole;
    else if (roleName == "l") role = LRole;
    if (role < 0) return false;
    return setData(index(row, 0), value, role);
}


bool BlocksListModel::insertRows(int row, int count, const QModelIndex &parent) {
    beginInsertRows(parent, row, row + count - 1);

    // No separate type storage to update
    endInsertRows();

    // Similarly: after insert/remove/moveRows or clear, call updateDocumentBeginCache().
    updateDocumentCache();

    return true;
}

bool BlocksListModel::moveRows(const QModelIndex &sourceParent, const int sourceRow, int count, const QModelIndex &destinationParent, const int destinationRow) {
    int newDestination = destinationRow;
    if (sourceRow < destinationRow)
        newDestination++;

    beginMoveRows(sourceParent, sourceRow, sourceRow, destinationParent, newDestination);

    m_blocks.move(sourceRow, destinationRow);

    endMoveRows();

    updateDocumentCache();

    return true;
}

bool BlocksListModel::removeRows(const int row, const int count, const QModelIndex &parent) {
    beginRemoveRows(parent, row, row + count - 1);

    for (int i = 0; i < count; ++i) {
        m_blocks.removeAt(row);
    }

    endRemoveRows();

    // Similarly: after insert/remove/moveRows or clear, call updateDocumentBeginCache().
    updateDocumentCache();

    return true;
}

bool BlocksListModel::clearModel(const int rowNumber) {
    const int previousRow = rowNumber - 1;
    beginRemoveRows(QModelIndex(), 0, previousRow);

    m_blocks.remove(0, previousRow);

    endRemoveRows();

    updateDocumentCache();

    return true;
}
