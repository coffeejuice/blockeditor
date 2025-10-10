#ifndef BLOCKSLISTMODEL_H
#define BLOCKSLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QQmlEngine>
#include <variant>
#include <QJsonObject>

// Base class for JSON (de)serialization
struct JsonSerializable {
    virtual ~JsonSerializable() = default;
    virtual QJsonObject toJson() const = 0;
    virtual void fromJson(const QJsonObject& o) = 0;
};

struct Document : public JsonSerializable {
    QString name, material_id, mesh_elements, weight;

    static constexpr const char* type() { return "document"; }
    static constexpr int fieldCount()   { return 4; }
    QString get(const int slot) const {
        switch (slot) {
        case 0: return name;
        case 1: return material_id;
        case 2: return mesh_elements;
        case 3: return weight;
        default: return {};
        }
    }
    bool set(const int slot, const QString& v) {
        switch (slot) {
        case 0: if (name != v) { name = v; return true; }  return false;
        case 1: if (material_id  != v) { material_id  = v; return true; }  return false;
        case 2: if (mesh_elements != v) { mesh_elements = v; return true; }  return false;
        case 3: if (weight != v) { weight = v; return true; }  return false;
        default: return false;
        }
    }
    QJsonObject toJson() const override {
        return QJsonObject{
            {"a", name},
            {"b", material_id},
            {"c", mesh_elements},
            {"d", weight},
        };
    }
    void fromJson(const QJsonObject& o) override {
        name = o["a"].toString();
        material_id = o["b"].toString();
        mesh_elements = o["c"].toString();
        weight = o["d"].toString();
    }
};

struct Block : public JsonSerializable {
    QString
        name,
        press_id,
        die_assembly_id, top_die_id, bottom_die_id,
        feed_direction_id,
        feed_first, feed_middle, feed_last,
        speed_upsetting, speed_prolongation, speed_full_die;
    static constexpr const char* type() { return "block"; }
    static constexpr int fieldCount()   { return 12; }
    QString get(const int slot) const {
        switch (slot) {
        case 0: return name;
        case 1: return press_id;
        case 2: return die_assembly_id;
        case 3: return top_die_id;
        case 4: return bottom_die_id;
        case 5: return feed_direction_id;
        case 6: return feed_first;
        case 7: return feed_middle;
        case 8: return feed_last;
        case 9: return speed_upsetting;
        case 10: return speed_prolongation;
        case 11: return speed_full_die;
        default: return {};
        }
    }
    bool set(const int slot, const QString& v) {
        switch (slot) {
        case 0: if (name != v) { name = v; return true; }  return false;
        case 1: if (press_id  != v) { press_id  = v; return true; }  return false;
        case 2: if (die_assembly_id != v) { die_assembly_id = v; return true; }  return false;
        case 3: if (top_die_id != v) { top_die_id = v; return true; }  return false;
        case 4: if (bottom_die_id  != v) { bottom_die_id  = v; return true; }  return false;
        case 5: if (feed_direction_id != v) { feed_direction_id = v; return true; }  return false;
        case 6: if (feed_first != v) { feed_first = v; return true; }  return false;
        case 7: if (feed_middle  != v) { feed_middle  = v; return true; }  return false;
        case 8: if (feed_last != v) { feed_last = v; return true; }  return false;
        case 9: if (speed_upsetting != v) { speed_upsetting = v; return true; }  return false;
        case 10: if (speed_prolongation  != v) { speed_prolongation  = v; return true; }  return false;
        case 11: if (speed_full_die != v) { speed_full_die = v; return true; }  return false;
        default: return false;
        }
    }
    QJsonObject toJson() const override {
        return QJsonObject{
            {"a", name},
            {"b", press_id},
            {"c", die_assembly_id},
            {"d", top_die_id},
            {"e", bottom_die_id},
            {"f", feed_direction_id},
            {"g", feed_first},
            {"h", feed_middle},
            {"i", feed_last},
            {"j", speed_upsetting},
            {"k", speed_prolongation},
            {"l", speed_full_die},
        };
    }
    void fromJson(const QJsonObject& o) override {
        name = o["a"].toString();
        press_id = o["b"].toString();
        die_assembly_id = o["c"].toString();
        top_die_id = o["d"].toString();
        bottom_die_id = o["e"].toString();
        feed_direction_id = o["f"].toString();
        feed_first = o["g"].toString();
        feed_middle = o["h"].toString();
        feed_last = o["i"].toString();
        speed_upsetting = o["j"].toString();
        speed_prolongation = o["k"].toString();
        speed_full_die = o["l"].toString();
    }
};

struct Heat : public JsonSerializable {
    QString name, timeUnits, typeTimeTemperature;
    static constexpr const char* type() { return "heat"; }
    static constexpr int fieldCount()   { return 3; }
    QString get(const int slot) const {
        switch (slot) {
        case 0: return name;
        case 1: return timeUnits;
        case 2: return typeTimeTemperature;
        default: return {};
        }
    }
    bool set(const int slot, const QString& v) {
        switch (slot) {
        case 0: if (name != v) { name = v; return true; }  return false;
        case 1: if (timeUnits  != v) { timeUnits  = v; return true; }  return false;
        case 2: if (typeTimeTemperature != v) { typeTimeTemperature = v; return true; }  return false;
        default: return false;
        }
    }
    QJsonObject toJson() const override {
        return QJsonObject{
            {"a", name},
            {"b", timeUnits},
            {"c", typeTimeTemperature},
        };
    }
    void fromJson(const QJsonObject& o) override {
        name = o["a"].toString();
        timeUnits = o["b"].toString();
        typeTimeTemperature = o["c"].toString();
    }
};

struct Upset : public JsonSerializable {
    QString operations;
    static constexpr const char* type() { return "upset"; }
    static constexpr int fieldCount()   { return 1; }
    QString get(const int slot) const {
        switch (slot) {
        case 0: return operations;
        default: return {};
        }
    }
    bool set(const int slot, const QString& v) {
        switch (slot) {
        case 0: if (operations != v) { operations = v; return true; }  return false;
        default: return false;
        }
    }
    QJsonObject toJson() const override {
        return QJsonObject{
            {"a", operations},
        };
    }
    void fromJson(const QJsonObject& o) override {
        operations = o["a"].toString();
    }
};

struct Draw : public JsonSerializable {
    QString operations;
    static constexpr const char* type() { return "draw"; }
    static constexpr int fieldCount()   { return 1; }
    QString get(const int slot) const {
        switch (slot) {
        case 0: return operations;
        default: return {};
        }
    }
    bool set(const int slot, const QString& v) {
        switch (slot) {
        case 0: if (operations != v) { operations = v; return true; }  return false;
        default: return false;
        }
    }
    QJsonObject toJson() const override {
        return QJsonObject{
            {"a", operations},
        };
    }
    void fromJson(const QJsonObject& o) override {
        operations = o["a"].toString();
    }
};

// Polymorphic storage
using ItemVariant = std::variant<Document, Block, Heat, Upset, Draw>;

class BlocksListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("BlocksListModel must be instantinated in C++")
    Q_PROPERTY(QVariantMap document READ documentBegin NOTIFY documentChanged)

    QList<ItemVariant> m_blocks;
    QVariantMap m_documentBegin;  // cached documentBegin value

    static int slotFromRole(const int role) {
        switch (role) {
        case ARole: return 0;
        case BRole: return 1;
        case CRole: return 2;
        case DRole: return 3;
        case ERole: return 4;
        case FRole: return 5;
        case GRole: return 6;
        case HRole: return 7;
        case IRole: return 8;
        case JRole: return 9;
        case KRole: return 10;
        case LRole: return 11;
        default:    return -1;
        }
    }

public:
    enum Roles {
        TypeRole = Qt::UserRole + 1, // "document" | "block" | "heat" | "upset" | "draw"
        ARole, BRole, CRole, DRole, ERole, FRole, GRole, HRole, IRole, JRole, KRole, LRole,  // slots 0,1,2,3,4,5,6,7,8,9,10,11,12
        FieldCountRole
        // ContentRole,
    };
    Q_ENUM(Roles)

    explicit BlocksListModel(QObject *parent = nullptr);
    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex &index) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void appendDocument(
        const QString& name = {},
        const QString& material_id = {},
        const QString& mesh_elements = {},
        const QString& weight = {}
    );
    Q_INVOKABLE void appendBlock(
        const QString& name = {},
        const QString& press_id = {},
        const QString& die_assembly_id = {},
        const QString& top_die_id = {},
        const QString& bottom_die_id = {},
        const QString& feed_direction_id = {},
        const QString& feed_first = {},
        const QString& feed_middle = {},
        const QString& feed_last = {},
        const QString& speed_upsetting = {},
        const QString& speed_prolongation = {},
        const QString& speed_full_die = {}
    );
    Q_INVOKABLE void appendHeat(
        const QString& name = {},
        const QString& timeUnits = {},
        const QString& typeTimeTemperature = {}
    );
    Q_INVOKABLE void appendUpset(const QString& operations = {});
    Q_INVOKABLE void appendDraw(const QString& operations = {});

    // Q_INVOKABLE bool appendRow(int rowNumber);
    Q_INVOKABLE bool insertRows(int row, int count, const QModelIndex &parent) override;
    Q_INVOKABLE bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationRow) override;
    Q_INVOKABLE bool removeRows(int row, int count, const QModelIndex &parent) override;
    Q_INVOKABLE bool clearModel(int rowNumber);

    Q_INVOKABLE bool saveToFile(const QString& filePath) const;
    Q_INVOKABLE bool loadFromFile(const QString& filePath);
    Q_INVOKABLE void removeRowAt(int row);
    Q_INVOKABLE bool setField(int row, const QString& roleName, const QVariant& value);

    // 1) Property getter (reactive)
    QVariantMap documentBegin() const { return m_documentBegin; }
    // 2) Also expose an on-demand method if you prefer calling it from QML
    Q_INVOKABLE QVariantMap getDocumentBegin() const { return m_documentBegin; }
    void beginResetModelWithDocRecalc() { beginResetModel(); }
    void endResetModelWithDocRecalc()   { endResetModel(); updateDocumentCache(); }

signals:
    void documentChanged();

protected:
    // Make sure to call this whenever the model content changes in a way
    // that might affect the "documentBegin" row (insert/remove/setData/clear/move).
    void updateDocumentCache() {
        // Simplified cache update: emit change signal
        emit documentChanged();
    }
};

#endif // BLOCKSLISTMODEL_H
