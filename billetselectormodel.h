#ifndef BILLETSELECTORMODEL_H
#define BILLETSELECTORMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QQmlEngine>
#include "BaseItem.h"


class DieSelectorModel final : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("BilletSelectorModel must be instantinated in C++")
    Q_PROPERTY(QVariantMap documentBegin READ documentBegin NOTIFY documentBeginChanged)

    QStringList m_types;
    QList<std::unique_ptr<BaseItem>> m_blocks;
    QVariantMap m_documentBegin;  // cached documentBegin value

    void propagateParams(BaseItem* src, BaseItem* dst) const {
        if (!src || !dst) return; // check of null
        if (src == dst) return; // check if same object

        // Propagate DocParams shared pointer
        if (src->docParamsPtr() && !dst->docParamsPtr())
        {
            dst->setDocParamsPtr( src->docParamsPtr() );
        }


    };

    void propagateParamsTillEnd(const int startIndex)
    {
        if (startIndex < 0 || startIndex >= m_blocks.size() -1) return;

        for (auto it = m_blocks.begin() + strartIndex; it != m_blocks.end(); ++it)
        {
            propagateParams(m_blocks[i], m_blocks[i + 1]);
        }
    };

public:
    enum BlocksRoles {
        BlockTypeRole = Qt::UserRole + 1,
        BlockContentRole,
    };

    explicit DieSelectorModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    Q_INVOKABLE bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationRow) override;
    Q_INVOKABLE bool insertRows(int row, int count, const QModelIndex &parent) override;
    Q_INVOKABLE bool removeRows(int row, int count, const QModelIndex &parent) override;
    Q_INVOKABLE bool appendRow(int rowNumber);
    Q_INVOKABLE bool clearModel(int rowNumber);

    // 1) Property getter (reactive)
    QVariantMap documentBegin() const { return m_documentBegin; }
    // 2) Also expose an on-demand method if you prefer calling it from QML
    Q_INVOKABLE QVariantMap getDocumentBegin() const { return m_documentBegin; }
    void beginResetModelWithDocRecalc() { beginResetModel(); }
    void endResetModelWithDocRecalc()   { endResetModel(); updateDocumentBeginCache(); }

signals:
    void documentBeginChanged();

protected:
    // Make sure to call this whenever the model content changes in a way
    // that might affect the "documentBegin" row (insert/remove/setData/clear/move).
    void updateDocumentBeginCache() {
        QVariantMap found;
        for (int r = 0; r < rowCount(); ++r) {
            const QModelIndex idx = index(r, 0);
            const QString type = data(idx, BlockTypeRole).toString();
            if (type == QStringLiteral("documentBegin")) {
                found = data(idx, BlockContentRole).toMap();
                break;
            }
        }
        if (found != m_documentBegin) {
            m_documentBegin = found;
            emit documentBeginChanged();
        }
    }

};

#endif // BILLETSELECTORMODEL_H
