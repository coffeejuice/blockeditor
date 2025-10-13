#ifndef BLOCKS_H
#define BLOCKS_H

#include <QAbstractListModel>
#include <QObject>
#include <QQmlEngine>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariantMap>
#include <memory>

// ===================== Polymorphic base =====================
class BaseBlock {
public:
    virtual ~BaseBlock() = default;

    // identity
    [[nodiscard]] virtual QString type() const = 0;
    // number of text fields (mapped to roles a..l)
    [[nodiscard]] virtual int fieldCount() const = 0;

    // slot access (a..l map to indexes 0..11)
    [[nodiscard]] virtual QString get(int slot) const = 0;
    virtual bool set(int slot, const QString& v) = 0;

    // document-only extra roles. Default: not applicable.
    [[nodiscard]] virtual QStringList selectorAImages() const { return {}; }
    [[nodiscard]] virtual QStringList selectorBImages() const { return {}; }
    [[nodiscard]] virtual QStringList selectorCImages() const { return {}; }
    [[nodiscard]] virtual int selectorASelected() const { return -1; }
    [[nodiscard]] virtual int selectorBSelected() const { return -1; }
    [[nodiscard]] virtual int selectorCSelected() const { return -1; }
    virtual bool setSelectorSelected(const int which, const int value) { Q_UNUSED(which); Q_UNUSED(value); return false; }

    // serialization
    [[nodiscard]] virtual QJsonObject toJson() const = 0;
    virtual void fromJson(const QJsonObject& o) = 0;

    // payload assignment used by Qt.EditRole
    virtual void assignFromMap(const QVariantMap& content) = 0;

    // factory helper: returns unique_ptr<BaseBlock> or nullptr if type not matched
    static std::unique_ptr<BaseBlock> makeFromEditPayload(const QVariantMap& obj);
};

// ===================== Concrete types =====================

class DocumentBlock final : public BaseBlock {
public:
    QString name, material_id, mesh_elements, weight;
    QStringList aList, bList, cList;
    int aSel = -1, bSel = -1, cSel = -1;

    [[nodiscard]] QString type() const override { return "document"; }
    [[nodiscard]] int fieldCount() const override { return 4; }

    [[nodiscard]] QString get(const int slot) const override {
        switch (slot) {
            case 0: return name;
            case 1: return material_id;
            case 2: return mesh_elements;
            case 3: return weight;
            default: return {};
        }
    }
    bool set(const int slot, const QString& v) override {
        switch (slot) {
            case 0: if (name != v) { name = v; return true; } return false;
            case 1: if (material_id != v) { material_id = v; return true; } return false;
            case 2: if (mesh_elements != v) { mesh_elements = v; return true; } return false;
            case 3: if (weight != v) { weight = v; return true; } return false;
            default: return false;
        }
    }

    [[nodiscard]] QStringList selectorAImages() const override { return aList; }
    [[nodiscard]] QStringList selectorBImages() const override { return bList; }
    [[nodiscard]] QStringList selectorCImages() const override { return cList; }
    [[nodiscard]] int selectorASelected() const override { return aSel; }
    [[nodiscard]] int selectorBSelected() const override { return bSel; }
    [[nodiscard]] int selectorCSelected() const override { return cSel; }
    bool setSelectorSelected(const int which, const int value) override {
        switch (which) {
            case 0: if (aSel != value) { aSel = value; return true; } return false;
            case 1: if (bSel != value) { bSel = value; return true; } return false;
            case 2: if (cSel != value) { cSel = value; return true; } return false;
            default: return false;
        }
    }

    [[nodiscard]] QJsonObject toJson() const override {
        return QJsonObject{
            {"a", name},
            {"b", material_id},
            {"c", mesh_elements},
            {"d", weight},
            {"aList", QJsonArray::fromStringList(aList)},
            {"bList", QJsonArray::fromStringList(bList)},
            {"cList", QJsonArray::fromStringList(cList)},
            {"aSelected", aSel},
            {"bSelected", bSel},
            {"cSelected", cSel},
        };
    }
    void fromJson(const QJsonObject& o) override {
        name = o.value("a").toString();
        material_id = o.value("b").toString();
        mesh_elements = o.value("c").toString();
        weight = o.value("d").toString();
        auto toList = [](const QJsonValue& v) {
            QStringList out;
            if (v.isArray()) {
                const auto arr = v.toArray();
                out.reserve(arr.size());
                for (const auto& e : arr) out.push_back(e.toString());
            }
            return out;
        };
        aList = toList(o.value("aList"));
        bList = toList(o.value("bList"));
        cList = toList(o.value("cList"));
        aSel = o.value("aSelected").toInt(-1);
        bSel = o.value("bSelected").toInt(-1);
        cSel = o.value("cSelected").toInt(-1);
    }
    void assignFromMap(const QVariantMap& m) override {
        if (m.contains("name")) name = m.value("name").toString();
        if (m.contains("material_id")) material_id = m.value("material_id").toString();
        if (m.contains("mesh_elements")) mesh_elements = m.value("mesh_elements").toString();
        if (m.contains("weight")) weight = m.value("weight").toString();
        if (m.contains("aList")) aList = m.value("aList").toStringList();
        if (m.contains("bList")) bList = m.value("bList").toStringList();
        if (m.contains("cList")) cList = m.value("cList").toStringList();
        if (m.contains("aSelected")) aSel = m.value("aSelected").toInt();
        if (m.contains("bSelected")) bSel = m.value("bSelected").toInt();
        if (m.contains("cSelected")) cSel = m.value("cSelected").toInt();
    }
};

class BlockBlock final : public BaseBlock {
public:
    QString name, press_id, die_assembly_id, top_die_id, bottom_die_id,
            feed_direction_id, feed_first, feed_middle, feed_last,
            speed_upsetting, speed_prolongation, speed_full_die;

    [[nodiscard]] QString type() const override { return "block"; }
    [[nodiscard]] int fieldCount() const override { return 12; }

    [[nodiscard]] QString get(const int slot) const override {
        switch (slot) {
            case 0: return name; case 1: return press_id; case 2: return die_assembly_id;
            case 3: return top_die_id; case 4: return bottom_die_id; case 5: return feed_direction_id;
            case 6: return feed_first; case 7: return feed_middle; case 8: return feed_last;
            case 9: return speed_upsetting; case 10: return speed_prolongation; case 11: return speed_full_die;
            default: return {};
        }
    }
    bool set(const int slot, const QString& v) override {
        switch (slot) {
            case 0: if (name != v) { name = v; return true; } return false;
            case 1: if (press_id != v) { press_id = v; return true; } return false;
            case 2: if (die_assembly_id != v) { die_assembly_id = v; return true; } return false;
            case 3: if (top_die_id != v) { top_die_id = v; return true; } return false;
            case 4: if (bottom_die_id != v) { bottom_die_id = v; return true; } return false;
            case 5: if (feed_direction_id != v) { feed_direction_id = v; return true; } return false;
            case 6: if (feed_first != v) { feed_first = v; return true; } return false;
            case 7: if (feed_middle != v) { feed_middle = v; return true; } return false;
            case 8: if (feed_last != v) { feed_last = v; return true; } return false;
            case 9: if (speed_upsetting != v) { speed_upsetting = v; return true; } return false;
            case 10: if (speed_prolongation != v) { speed_prolongation = v; return true; } return false;
            case 11: if (speed_full_die != v) { speed_full_die = v; return true; } return false;
            default: return false;
        }
    }

    [[nodiscard]] QJsonObject toJson() const override {
        return QJsonObject{
            {"a", name}, {"b", press_id}, {"c", die_assembly_id}, {"d", top_die_id},
            {"e", bottom_die_id}, {"f", feed_direction_id}, {"g", feed_first},
            {"h", feed_middle}, {"i", feed_last}, {"j", speed_upsetting},
            {"k", speed_prolongation}, {"l", speed_full_die},
        };
    }
    void fromJson(const QJsonObject& o) override {
        name = o.value("a").toString(); press_id = o.value("b").toString();
        die_assembly_id = o.value("c").toString(); top_die_id = o.value("d").toString();
        bottom_die_id = o.value("e").toString(); feed_direction_id = o.value("f").toString();
        feed_first = o.value("g").toString(); feed_middle = o.value("h").toString();
        feed_last = o.value("i").toString(); speed_upsetting = o.value("j").toString();
        speed_prolongation = o.value("k").toString(); speed_full_die = o.value("l").toString();
    }
    void assignFromMap(const QVariantMap& m) override {
        if (m.contains("name")) name = m.value("name").toString();
        if (m.contains("press_id")) press_id = m.value("press_id").toString();
        if (m.contains("die_assembly_id")) die_assembly_id = m.value("die_assembly_id").toString();
        if (m.contains("top_die_id")) top_die_id = m.value("top_die_id").toString();
        if (m.contains("bottom_die_id")) bottom_die_id = m.value("bottom_die_id").toString();
        if (m.contains("feed_direction_id")) feed_direction_id = m.value("feed_direction_id").toString();
        if (m.contains("feed_first")) feed_first = m.value("feed_first").toString();
        if (m.contains("feed_middle")) feed_middle = m.value("feed_middle").toString();
        if (m.contains("feed_last")) feed_last = m.value("feed_last").toString();
        if (m.contains("speed_upsetting")) speed_upsetting = m.value("speed_upsetting").toString();
        if (m.contains("speed_prolongation")) speed_prolongation = m.value("speed_prolongation").toString();
        if (m.contains("speed_full_die")) speed_full_die = m.value("speed_full_die").toString();
    }
};

class HeatBlock final : public BaseBlock {
public:
    QString name, timeUnits, typeTimeTemperature;

    [[nodiscard]] QString type() const override { return "heat"; }
    [[nodiscard]] int fieldCount() const override { return 3; }

    [[nodiscard]] QString get(int slot) const override {
        switch (slot) { case 0: return name; case 1: return timeUnits; case 2: return typeTimeTemperature; default: return {}; }
    }
    bool set(int slot, const QString& v) override {
        switch (slot) { case 0: if (name != v) { name = v; return true; } return false;
                        case 1: if (timeUnits != v) { timeUnits = v; return true; } return false;
                        case 2: if (typeTimeTemperature != v) { typeTimeTemperature = v; return true; } return false;
                        default: return false; }
    }

    [[nodiscard]] QJsonObject toJson() const override {
        return QJsonObject{ {"a", name}, {"b", timeUnits}, {"c", typeTimeTemperature} };
    }
    void fromJson(const QJsonObject& o) override {
        name = o.value("a").toString(); timeUnits = o.value("b").toString(); typeTimeTemperature = o.value("c").toString();
    }
    void assignFromMap(const QVariantMap& m) override {
        if (m.contains("name")) name = m.value("name").toString();
        if (m.contains("timeUnits")) timeUnits = m.value("timeUnits").toString();
        if (m.contains("typeTimeTemperature")) typeTimeTemperature = m.value("typeTimeTemperature").toString();
    }
};

class UpsetBlock final : public BaseBlock {
public:
    QString operations;

    [[nodiscard]] QString type() const override { return "upset"; }
    [[nodiscard]] int fieldCount() const override { return 1; }

    [[nodiscard]] QString get(int slot) const override { return slot == 0 ? operations : QString(); }
    bool set(int slot, const QString& v) override {
        if (slot == 0 && operations != v) { operations = v; return true; }
        return false;
    }

    [[nodiscard]] QJsonObject toJson() const override { return QJsonObject{ {"a", operations} }; }
    void fromJson(const QJsonObject& o) override { operations = o.value("a").toString(); }
    void assignFromMap(const QVariantMap& m) override {
        if (m.contains("operations")) operations = m.value("operations").toString();
    }
};

class DrawBlock final : public BaseBlock {
public:
    QString operations;

    [[nodiscard]] QString type() const override { return "draw"; }
    [[nodiscard]] int fieldCount() const override { return 1; }

    [[nodiscard]] QString get(int slot) const override { return slot == 0 ? operations : QString(); }
    bool set(int slot, const QString& v) override {
        if (slot == 0 && operations != v) { operations = v; return true; }
        return false;
    }

    [[nodiscard]] QJsonObject toJson() const override { return QJsonObject{ {"a", operations} }; }
    void fromJson(const QJsonObject& o) override { operations = o.value("a").toString(); }
    void assignFromMap(const QVariantMap& m) override {
        if (m.contains("operations")) operations = m.value("operations").toString();
    }
};

#endif // BLOCKS_H
