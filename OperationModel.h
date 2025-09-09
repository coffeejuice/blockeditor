#pragma once
#include <QAbstractListModel>
#include <QString>
#include <QVector>

struct operationLib {
    int index;
    QString label;
};

class OperationModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles { OpIndexRole = Qt::UserRole + 1, LabelRole };

    explicit OperationModel(QObject* parent = nullptr)
        : QAbstractListModel(parent) {}

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        if (parent.isValid()) return 0;
        return m_ops.size();
    }

    QVariant data(const QModelIndex& idx, int role) const override {
        if (!idx.isValid() || idx.row() < 0 || idx.row() >= m_ops.size())
            return {};
        const auto& op = m_ops[idx.row()];
        switch (role) {
        case OpIndexRole: return op.index;
        case LabelRole:   return op.label;
        default:          return {};
        }
    }

    QHash<int, QByteArray> roleNames() const override {
        return {
            { OpIndexRole, "opIndex" },
            { LabelRole,   "label"   }
        };
    }

    void setOperations(QVector<operationLib> ops) {
        beginResetModel();
        m_ops = std::move(ops);
        endResetModel();
    }

private:
    QVector<operationLib> m_ops;
};
