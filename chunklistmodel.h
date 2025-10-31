#ifndef CHUNKLISTMODEL_H
#define CHUNKLISTMODEL_H

#pragma once
#include <QAbstractListModel>
#include <QQmlEngine>
#include <QVector>
#include "fieldlistmodel.h"

class ChunkListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("ChunkListModel is provided by CardsModel; do not create in QML.")

    QVector<FieldListModel*> m_chunks;  // each chunk is a FieldListModel

public:
    explicit ChunkListModel(QObject *parent = nullptr);

    enum Roles {FieldsRole = Qt::UserRole + 1};
    Q_ENUM(Roles)

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    // QML-callable CRUD on chunks
    Q_INVOKABLE void appendChunk();
    Q_INVOKABLE void insertChunk(int row);
    Q_INVOKABLE void removeChunk(int row);
    Q_INVOKABLE QObject* fieldsAt(int row) const; // QML internal

public:
    // C++-internal helper
    FieldListModel* fieldsAtRaw(int row) const; // C++-internal
};
Q_DECLARE_METATYPE(ChunkListModel*)
#endif // CHUNKLISTMODEL_H
