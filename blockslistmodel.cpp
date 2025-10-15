// blockslistmodel.cpp
#include <algorithm>
#include <utility>
#include "blockslistmodel.h"
#include <QDir>
#include <QUrl>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonArray>

// ===================== Base factory =====================
std::unique_ptr<BaseBlock> BaseBlock::makeFromEditPayload(const QVariantMap& obj) {
    const auto t = obj.value("type").toString();
    const auto content = obj.value("content").toMap();
    std::unique_ptr<BaseBlock> p;
    if (t == "document") p = std::make_unique<DocumentBlock>();
    else if (t == "block") p = std::make_unique<BlockBlock>();
    else if (t == "heat") p = std::make_unique<HeatBlock>();
    else if (t == "upset") p = std::make_unique<UpsetBlock>();
    else if (t == "draw") p = std::make_unique<DrawBlock>();
    else return nullptr;
    p->assignFromMap(content);
    return p;
}

// ===================== Model impl =====================

BlocksListModel::BlocksListModel(QObject* parent) : QAbstractListModel(parent) {
    appendDocument(
        "100.0342.0",
        "Inconel718",
        "10",
        "2000",
        {
            "rectangle_off.svg","rectangle_on.svg",
            "square_off.svg","square_on.svg",
            "octagon_off.svg","octagon_on.svg",
            "polygon16_off.svg","polygon16_on.svg",
            "round_off.svg","round_off.svg"},
        {"square_chamfers_sharp_off.svg","square_chamfers_equal_off.svg","square_chamfers_different_off.svg"},
        {"square_chamfers_sharp_on.svg","square_chamfers_equal_on.svg","square_chamfers_different_on.svg"},
        0,
        1,
        2
        );
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
    appendUpset("(1200)->900->750");
    appendDraw("(550)->500->(90)550->(90)515->(90)430->(45)570->(45)4900");
    appendUpset("(900)->750->600");
    appendDraw("(650)->580->(90)610->(90)585->(90)490->(45)530->(45)540");
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
// REMOVE obsolete std::variant-based helpers and use virtual calls instead.

// Old (obsolete):
// static QString typeOf(const ItemVariant& v) { ... }
// static int fieldCountOf(const ItemVariant& v) { ... }
// static QString getSlot(const ItemVariant& v, int slot) { ... }
// static bool setSlot(ItemVariant& v, int slot, const QString& value) { ... }

// Replace usages below:

QVariant BlocksListModel::data(const QModelIndex &index, const int role) const {

    if (!checkIndex(index, CheckIndexOption::IndexIsValid)) return {};

    // Safe read: bind to const ref, then get()
    const std::unique_ptr<BaseBlock>& u = m_blocks.at(index.row());
    BaseBlock* const ptr = u.get();
    if (!ptr) return {};

    if (role == TypeRole)           return ptr->type();
    if (role == FieldCountRole)     return ptr->fieldCount();
    if (role == AListRole)          return ptr->selectorAImages();
    if (role == BListRole)          return ptr->selectorBImages();
    if (role == CListRole)          return ptr->selectorCImages();
    if (role == ASelectedRole)      return ptr->selectorASelected();
    if (role == BSelectedRole)      return ptr->selectorBSelected();
    if (role == CSelectedRole)      return ptr->selectorCSelected();

    if (const int slot = slotFromTextFieldRole(role); slot >= 0) return ptr->get(slot);

    return {};
}

bool BlocksListModel::setData(const QModelIndex &index, const QVariant &value, int role) {

    if (!checkIndex(index, CheckIndexOption::IndexIsValid)) return false;

    // Read current ptr via const-ref; do not bind non-const ref to avoid copies
    BaseBlock* ptr = m_blocks.at(index.row()).get();
    if (!ptr) return false;

    bool changed = false;

    if (role == ASelectedRole || role == BSelectedRole || role == CSelectedRole) {
        const int which = (role == ASelectedRole ? 0 : role == BSelectedRole ? 1 : 2);
        changed = ptr->setSelectorSelected(which, value.toInt());
    } else if (const int slot = slotFromTextFieldRole(role); slot >= 0) {
        changed = ptr->set(slot, value.toString());
    } else if (role == Qt::EditRole) {
        const auto obj = value.toMap();
        std::unique_ptr<BaseBlock> newItem = BaseBlock::makeFromEditPayload(obj);
        if (!newItem) return false;
        m_blocks[index.row()] = std::move(newItem);   // deletes the old pointee now
        emit dataChanged(index, index, allRoleIds());
        updateDocumentCache();
        return true;
    }

    if (changed) emit dataChanged(index, index, { role });
    return changed;
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
            { FieldCountRole, "fieldCount" },
            { AListRole,      "aList" },
            { BListRole,      "bList" },
            { CListRole,      "cList" },
            { ASelectedRole,  "aSelected" },
            { BSelectedRole,  "bSelected" },
            { CSelectedRole,  "cSelected" }
    };
    return mapping;
}
// QML helpers
void BlocksListModel::appendDocument(
    const QString& name,
    const QString& material_id,
    const QString& mesh_elements,
    const QString& weight,
    QStringList selectorAImages,
    QStringList selectorBImages,
    QStringList selectorCImages,
    const int aSel,
    const int bSel,
    const int cSel
) {
    const int r = static_cast<int>(m_blocks.size());
    beginInsertRows({}, r, r);
    auto d = std::make_unique<DocumentBlock>();
    d->name = name;
    d->material_id = material_id;
    d->mesh_elements = mesh_elements;
    d->weight = weight;
    d->aList = std::move(selectorAImages);
    d->bList = std::move(selectorBImages);
    d->cList = std::move(selectorCImages);
    d->aSel = aSel;
    d->bSel = bSel;
    d->cSel = cSel;
    m_blocks.push_back(std::move(d));
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
    auto b = std::make_unique<BlockBlock>();
    b->name = name;
    b->press_id = press_id;
    b->die_assembly_id = die_assembly_id;
    b->top_die_id = top_die_id;
    b->bottom_die_id = bottom_die_id;
    b->feed_direction_id = feed_direction_id;
    b->feed_first = feed_first;
    b->feed_middle = feed_middle;
    b->feed_last = feed_last;
    b->speed_upsetting = speed_upsetting;
    b->speed_prolongation = speed_prolongation;
    b->speed_full_die = speed_full_die;
    m_blocks.push_back(std::move(b));
    endInsertRows();
}
void BlocksListModel::appendHeat(const QString& name, const QString& timeUnits, const QString& typeTimeTemperature) {
    const int r = static_cast<int>(m_blocks.size());
    beginInsertRows({}, r, r);
    auto h = std::make_unique<HeatBlock>();
    h->name = name;
    h->timeUnits = timeUnits;
    h->typeTimeTemperature = typeTimeTemperature;
    m_blocks.push_back(std::move(h));
    endInsertRows();
}
void BlocksListModel::appendUpset(const QString& operations) {
    const int r = static_cast<int>(m_blocks.size());
    beginInsertRows({}, r, r);
    auto u = std::make_unique<UpsetBlock>();
    u->operations = operations;
    m_blocks.push_back(std::move(u));
    endInsertRows();
}
void BlocksListModel::appendDraw(const QString& operations) {
    const int r = static_cast<int>(m_blocks.size());
    beginInsertRows({}, r, r);
    auto d = std::make_unique<DrawBlock>();
    d->operations = operations;
    m_blocks.push_back(std::move(d));
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
    m_blocks.erase(m_blocks.begin() + row);
    endRemoveRows();
    updateDocumentCache();
}

bool BlocksListModel::saveToFile() const {
    // Accept both local paths and file URLs
    QString p = QString("C:/Users/alext/OneDrive/Documents/test.json");
    if (p.startsWith("file:/")) {
        const QUrl u(p);
        if (u.isLocalFile())
            p = u.toLocalFile();
    }
    // Normalize separators to native
    p = QDir::toNativeSeparators(p);

    QFile f(p);
    if (!f.open(QIODevice::WriteOnly)) return false;

    QJsonArray arr;
    for (const auto& ptr : m_blocks) {
        if (!ptr) continue;
        QJsonObject o;
        o["type"] = ptr->type();
        const QJsonObject per = ptr->toJson();
        for (auto it = per.begin(); it != per.end(); ++it) o.insert(it.key(), it.value());
        arr.append(o);
    }
    f.write(QJsonDocument(arr).toJson(QJsonDocument::Indented));
    return true;
}

bool BlocksListModel::loadFromFile() {
    QString p = QString("C:/Users/alext/OneDrive/Documents/test.json");
    if (p.startsWith("file:/")) {
        const QUrl u(p);
        if (u.isLocalFile())
            p = u.toLocalFile();
    }
    p = QDir::toNativeSeparators(p);

    QFile f(p);
    if (!f.open(QIODevice::ReadOnly)) return false;
    const auto doc = QJsonDocument::fromJson(f.readAll());
    if (!doc.isArray()) return false;

    const auto arr = doc.array();
    beginResetModel();
    m_blocks.clear();
    for (const auto& v : arr) {
        const auto o = v.toObject();
        const auto t = o.value("type").toString();
        std::unique_ptr<BaseBlock> pItem;
        if (t == "document")   pItem = std::make_unique<DocumentBlock>();
        else if (t == "block") pItem = std::make_unique<BlockBlock>();
        else if (t == "heat")  pItem = std::make_unique<HeatBlock>();
        else if (t == "upset") pItem = std::make_unique<UpsetBlock>();
        else if (t == "draw")  pItem = std::make_unique<DrawBlock>();
        else throw std::runtime_error(("Unknown block type: " + t).toStdString());
        pItem->fromJson(o);
        m_blocks.push_back(std::move(pItem));
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


bool BlocksListModel::insertRows(const int row, const int count, const QModelIndex &parent) {
    beginInsertRows(parent, row, row + count - 1);

    // No separate type storage to update
    endInsertRows();

    // Similarly: after insert/remove/moveRows or clear, call updateDocumentBeginCache().
    updateDocumentCache();

    return true;
}

bool BlocksListModel::moveRows(const QModelIndex &sourceParent, const int from, const int count, const QModelIndex &destinationParent, const int to) {
    if (count <= 0 || sourceParent != destinationParent) return false;

    const int total = static_cast<int>(m_blocks.size());
    if (from < 0 || from + count > total || to < 0 || to > total) return false;

    // When moving "down", destinationRow refers to the position *before* removal,
    // so it must be strictly greater than the moved block's range. When moving up,
    // it must not fall inside the source range either.
    if (to >= from && to <= from + count) return false;

    beginMoveRows(sourceParent, from, from + count - 1, destinationParent, to);

    const auto first = m_blocks.begin();
    if (from < to) {
        std::rotate(
            first + from,
            first + from + count,
            first + to);
    } else {
        std::rotate(
            first + to,
            first + from,
            first + from + count);
    }

    endMoveRows();
    updateDocumentCache();
    return true;
}

bool BlocksListModel::moveRowTo(const int from, int to) {
    const int total = static_cast<int>(m_blocks.size());
    if (from < 0 || from >= total || total <= 1 ) return false;

    if (to < 0) to = 0;
    if (to >= total) to = total - 1;

    if (to == from) return false;

    const int insert_to = (from < to) ? to + 1 : to;

    // When moving "down", "to" refers to the position *before* removal,
    // so it must be strictly greater than the moved block's range. When moving up,
    // it must not fall inside the source range either.
    if (insert_to >= from && insert_to <= from + 1) return false;

    beginMoveRows({}, from, from, {}, insert_to);

    const auto it_from = m_blocks.begin() + from;
    const auto it_to = m_blocks.begin() + insert_to;
    if (from < insert_to) {
        std::rotate(it_from, it_from, it_to);
    } else {
        std::rotate(it_to, it_from, it_from);
    }

    endMoveRows();
    updateDocumentCache();
    return true;
}

bool BlocksListModel::removeRows(const int row, const int count, const QModelIndex &parent) {
    beginRemoveRows(parent, row, row + count - 1);

    for (int i = 0; i < count; ++i) {
        m_blocks.erase(m_blocks.begin() + row);
    }

    endRemoveRows();

    // Similarly: after insert/remove/moveRows or clear, call updateDocumentBeginCache().
    updateDocumentCache();

    return true;
}

bool BlocksListModel::clearModel(const int rowNumber) {
    const int previousRow = rowNumber - 1;
    beginRemoveRows(QModelIndex(), 0, previousRow);

    m_blocks.erase(m_blocks.begin() + previousRow);

    endRemoveRows();

    updateDocumentCache();

    return true;
}

void BlocksListModel::setSelectorSelected(const int row, const QString& roleName, const int value) {
    if (row < 0 || row >= m_blocks.size()) return;
    int role = -1;
    if (roleName == "a") role = ASelectedRole;
    else if (roleName == "b") role = BSelectedRole;
    else if (roleName == "c") role = CSelectedRole;
    if (role < 0) return;
    setData(this->index(row, 0), value, role);
}
