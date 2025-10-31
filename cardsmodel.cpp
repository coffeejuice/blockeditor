#include "cardsmodel.h"

CardsModel::CardsModel(QObject *parent) : QAbstractListModel{parent} {
    // Demo content
    appendCard("Alpha");
    chunksAtRaw(0)->appendChunk();
    chunksAtRaw(0)->appendChunk();
    chunksAtRaw(0)->fieldsAtRaw(0)->append("A1", "foo");
    chunksAtRaw(0)->fieldsAtRaw(0)->append("A2", "bar");
    chunksAtRaw(0)->fieldsAtRaw(1)->append("A3", "baz");

    appendCard("Beta");
    chunksAtRaw(1)->appendChunk();
    chunksAtRaw(1)->fieldsAtRaw(0)->append("B1",  42);
    chunksAtRaw(1)->fieldsAtRaw(0)->append("B2",  true);

    appendCard("Gamma");
    chunksAtRaw(2)->appendChunk();
    chunksAtRaw(2)->appendChunk();
    chunksAtRaw(2)->appendChunk();
    chunksAtRaw(2)->fieldsAtRaw(2)->append("G-last", "tail");
}

int CardsModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : m_cards.size();
}

QVariant CardsModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row()<0 || index.row()>=m_cards.size()) return {};
    const auto& c = m_cards.at(index.row());
    switch (role) {
        case TitleRole:     return c.title;
        case ChunksRole:    return QVariant::fromValue(static_cast<QObject*>(c.chunks));
        default:            return {};
    }
}

QHash<int, QByteArray> CardsModel::roleNames() const {
    return { { TitleRole, "title"}, { ChunksRole, "chunks" } };
}

void CardsModel::appendCard(const QString &title) {
    const int r = m_cards.size();
    beginInsertRows({}, r, r);
    Card c;
    c.title = title;
    c.chunks = new ChunkListModel(this); // owned by CardsModel
    m_cards.push_back(c);
    endInsertRows();
}

void CardsModel::removeCard(int row) {
    if (row<0 || row>=m_cards.size()) return;
    beginRemoveRows({}, row, row);
    auto* cm = m_cards[row].chunks;
    if (cm) cm->deleteLater();
    endRemoveRows();
}

QObject* CardsModel::chunksAt(int row) const {
    return static_cast<QObject*>(chunksAtRaw(row));
}

ChunkListModel* CardsModel::chunksAtRaw(int row) const {
    if (row < 0 || row >= m_cards.size()) return nullptr;
    return m_cards[row].chunks;
}
