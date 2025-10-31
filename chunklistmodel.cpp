#include "chunklistmodel.h"
#include <algorithm>

ChunkListModel::ChunkListModel(QObject *parent) : QAbstractListModel{parent} {}

int ChunkListModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : m_chunks.size();
}

QVariant ChunkListModel::data(const QModelIndex &index, int role) const {
    if (role == FieldsRole)
        return QVariant::fromValue(static_cast<QObject*>(m_chunks.at(index.row())));
    return {};
}

QHash<int, QByteArray> ChunkListModel::roleNames() const {
    return { { FieldsRole, "fields" } };
}

void ChunkListModel::appendChunk() {
    const int r = m_chunks.size();
    beginInsertRows({}, r, r);
    auto* fld = new FieldListModel(const_cast<ChunkListModel*>(this));
    m_chunks.push_back(fld);
    endInsertRows();
}

void ChunkListModel::insertChunk(int row) {
    const int r = std::clamp(row, 0, static_cast<int>(m_chunks.size()));
    beginInsertRows({}, r, r);
    auto* fld = new FieldListModel(const_cast<ChunkListModel*>(this));
    m_chunks.insert(m_chunks.begin()+r, fld);
    endInsertRows();
}

void ChunkListModel::removeChunk(int row) {
    if (row<0 || row>=m_chunks.size()) return;
    beginRemoveRows({}, row, row);
    auto* m = m_chunks[row];
    m_chunks.removeAt(row);
    if (m) m->deleteLater();
    endRemoveRows();
}

QObject *ChunkListModel::fieldsAt(int row) const {
    return static_cast<QObject*>(fieldsAtRaw(row));
}

FieldListModel* ChunkListModel::fieldsAtRaw(int row) const {
    if (row < 0 || row >= m_chunks.size()) return nullptr;
    return m_chunks[row];
}
