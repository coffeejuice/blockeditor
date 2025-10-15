#ifndef BLOCKSLISTMODEL_H
#define BLOCKSLISTMODEL_H

#include <QObject>
#include <QQmlEngine>
#include <memory>

#include "blocks.h"

class BlocksListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("BlocksListModel must be instantinated in C++")
    Q_PROPERTY(QVariantMap document READ documentBegin NOTIFY documentChanged)

    std::vector<std::unique_ptr<BaseBlock>> m_blocks;
    QVariantMap m_documentBegin;

    static int slotFromTextFieldRole(const int role) {
        switch (role) {
        case ARole: return 0; case BRole: return 1; case CRole: return 2; case DRole: return 3;
        case ERole: return 4; case FRole: return 5; case GRole: return 6; case HRole: return 7;
        case IRole: return 8; case JRole: return 9; case KRole: return 10; case LRole: return 11;
        default:    return -1;
        }
    }

public:
    enum Roles {
        TypeRole = Qt::UserRole + 1,
        ARole, BRole, CRole, DRole, ERole, FRole, GRole, HRole, IRole, JRole, KRole, LRole,
        FieldCountRole,
        AListRole, BListRole, CListRole,
        ASelectedRole, BSelectedRole, CSelectedRole,
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
        const QString& weight = {},
        QStringList selectorAImages = {},
        QStringList selectorBImages = {},
        QStringList selectorCImages = {},
        int aSel = -1,
        int bSel = -1,
        int cSel = -1
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

    Q_INVOKABLE bool insertRows(int row, int count, const QModelIndex &parent) override;
    Q_INVOKABLE bool moveRows(const QModelIndex &sourceParent, int from, int count, const QModelIndex &destinationParent, int to) override;
    Q_INVOKABLE bool moveRowTo(int from, int to);
    Q_INVOKABLE bool removeRows(int row, int count, const QModelIndex &parent) override;
    Q_INVOKABLE bool clearModel(int rowNumber);

    Q_INVOKABLE [[nodiscard]] bool saveToFile() const;
    Q_INVOKABLE bool loadFromFile();
    Q_INVOKABLE void removeRowAt(int row);
    Q_INVOKABLE bool setField(int row, const QString& roleName, const QVariant& value);
    Q_INVOKABLE void setSelectorSelected(int row, const QString& roleName, int value);

    [[nodiscard]] QVariantMap documentBegin() const { return m_documentBegin; }
    Q_INVOKABLE [[nodiscard]] QVariantMap getDocumentBegin() const { return m_documentBegin; }
    void beginResetModelWithDocRecalc() { beginResetModel(); }
    void endResetModelWithDocRecalc()   { endResetModel(); updateDocumentCache(); }

    [[nodiscard]] QVector<int> allRoleIds() const {
        QVector<int> roles;
        const auto rn = roleNames();
        roles.reserve(rn.size());
        for (auto it = rn.cbegin(); it != rn.cend(); ++it) roles.push_back(it.key());
        return roles;
    }

signals:
    void documentChanged();

protected:
    void updateDocumentCache()
    {
        emit documentChanged();
        const bool a = saveToFile();
    }
};

#endif // BLOCKSLISTMODEL_H
