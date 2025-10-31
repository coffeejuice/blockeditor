#ifndef CARDSMODEL_H
#define CARDSMODEL_H

#include <QAbstractListModel>
#include <QQmlEngine>
#include <QVector>
#include <QString>
#include "chunklistmodel.h"

class CardsModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Instantinate in c++ and pass to QML")

    struct Card {
        QString title;
        ChunkListModel* chunks = nullptr;
    };
    QVector<Card> m_cards;

public:
    explicit CardsModel(QObject *parent = nullptr);

    enum Roles {TitleRole = Qt::UserRole+1, ChunksRole};
    Q_ENUM(Roles)

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    // QML CRUD for cards
    Q_INVOKABLE void appendCard(const QString& title);
    Q_INVOKABLE void removeCard(int row);
    Q_INVOKABLE QObject* chunksAt(int row) const;  // QML-facing

public:
    // C++-internal helper (strongly typed)
    ChunkListModel* chunksAtRaw(int row) const;  // C++-internal
};

#endif // CARDSMODEL_H
