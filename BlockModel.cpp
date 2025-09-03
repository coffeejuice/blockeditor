#include "BlockModel.h"
#include <QUrl>

BlockModel::BlockModel(QObject* parent)
    : QAbstractListModel(parent)
{
    // optional: seed a couple of demo rows
    m.push_back({ Paragraph, 0, QStringLiteral("Hello, block editor!"), QUrl() });
    m.push_back({ Image,     0, QString(), QUrl() });
}

int BlockModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) return 0;
    return m.size();
}

QVariant BlockModel::data(const QModelIndex& idx, int role) const {
    if (!idx.isValid() || idx.row() < 0 || idx.row() >= m.size())
        return {};
    const Block& b = m[idx.row()];
    switch (role) {
    case TypeRole:      return static_cast<int>(b.type);
    case IndentRole:    return b.indent;
    case TextRole:      return b.text;
    case ImageUrlRole:  return b.imageUrl;
    default:            return {};
    }
}

bool BlockModel::setData(const QModelIndex& idx, const QVariant& v, int role) {
    if (!idx.isValid() || idx.row() < 0 || idx.row() >= m.size())
        return false;
    Block& b = m[idx.row()];
    bool changed = false;
    switch (role) {
    case TypeRole: {
        const int t = v.toInt();
        if (t >= Paragraph && t <= Image && b.type != static_cast<BlockType>(t)) {
            b.type = static_cast<BlockType>(t);
            changed = true;
        }
        break;
    }
    case IndentRole: {
        const int ind = v.toInt();
        if (b.indent != ind) { b.indent = ind; changed = true; }
        break;
    }
    case TextRole: {
        const QString s = v.toString();
        if (b.text != s) { b.text = s; changed = true; }
        break;
    }
    case ImageUrlRole: {
        const QUrl u = v.toUrl();
        if (b.imageUrl != u) { b.imageUrl = u; changed = true; }
        break;
    }
    default: break;
    }
    if (changed) {
        emit dataChanged(idx, idx, { role });
        return true;
    }
    return false;
}

QHash<int, QByteArray> BlockModel::roleNames() const {
    QHash<int, QByteArray> r;
    r[TypeRole]     = "blockType";   // <-- exact names used in QML
    r[IndentRole]   = "indent";
    r[TextRole]     = "text";
    r[ImageUrlRole] = "imageUrl";
    return r;
}


void BlockModel::appendParagraph(const QString& text, int indent) {
    const int row = m.size();
    beginInsertRows(QModelIndex(), row, row);
    m.push_back({ Paragraph, indent, text, QUrl() });
    endInsertRows();
}

void BlockModel::appendImage(const QUrl& src, int indent) {
    const int row = m.size();
    beginInsertRows(QModelIndex(), row, row);
    m.push_back({ Image, indent, QString(), src });
    endInsertRows();
}

void BlockModel::convertType(int row, BlockType t) {
    if (row < 0 || row >= m.size()) return;
    if (m[row].type == t) return;
    m[row].type = t;
    const QModelIndex idx = index(row);
    emit dataChanged(idx, idx, { TypeRole });
}

// Optional: enable reordering
bool BlockModel::moveRows(const QModelIndex& srcParent, int srcRow, int count,
                          const QModelIndex& dstParent, int dstRow) {
    if (srcParent.isValid() || dstParent.isValid()) return false;
    if (count <= 0 || srcRow < 0 || srcRow + count > m.size()) return false;
    if (dstRow < 0 || dstRow > m.size() || (dstRow >= srcRow && dstRow <= srcRow + count))
        return false;

    beginMoveRows(QModelIndex(), srcRow, srcRow + count - 1, QModelIndex(), dstRow);
    QVector<Block> chunk;
    chunk.reserve(count);
    for (int i = 0; i < count; ++i) chunk.push_back(m.takeAt(srcRow));
    int ins = dstRow > srcRow ? (dstRow - count) : dstRow;
    for (int i = 0; i < chunk.size(); ++i) m.insert(ins + i, chunk[i]);
    endMoveRows();
    return true;
}
