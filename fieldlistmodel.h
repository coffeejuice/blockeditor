#ifndef FIELDLISTMODEL_H
#define FIELDLISTMODEL_H

#pragma once
#include <QAbstractListModel>
#include <QQmlEngine>
#include <QVector>
#include <QString>
#include <QVariant>

class FieldListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("FieldListModel is provided by ChunkListModel; do not create in QML.")

    struct Field {
        QString label;
        QVariant value;
        bool editable=true;
        QString key;
    };
    QVector<Field> m_fields;

public:
    enum Roles {LabelRole = Qt::UserRole + 1, ValueRole, EditableRole, KeyRole};
    Q_ENUM(Roles);

    explicit FieldListModel(QObject* parent=nullptr);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    Q_INVOKABLE void append(const QString& label=QString(),
                            const QVariant& value=QVariant(),
                            bool editable=true,
                            const QString& key=QString());
    Q_INVOKABLE void insert(int row,
                            const QString& label=QString(),
                            const QVariant& value=QVariant(),
                            bool editable=true,
                            const QString& key=QString());
    Q_INVOKABLE void remove(int row);
    Q_INVOKABLE void clear();
    Q_INVOKABLE bool setValueAt(int row, const QVariant &value);
};
Q_DECLARE_METATYPE(FieldListModel*)
#endif // FIELDLISTMODEL_H
