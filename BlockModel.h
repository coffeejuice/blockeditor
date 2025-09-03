// BlockModel.h
#pragma once
#include <QAbstractListModel>
#include <QVector>
#include <QUrl>          // <-- REQUIRED
#include <QHash>
#include <QByteArray>


class BlockModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum BlockType { Paragraph, Todo, Heading, Kanban, Image };
    Q_ENUM(BlockType)

    enum Roles { TypeRole = Qt::UserRole+1, IndentRole, TextRole, ImageUrlRole };

    explicit BlockModel(QObject* parent=nullptr);
    int rowCount(const QModelIndex& p = {}) const override;
    QVariant data(const QModelIndex& i, int role) const override;
    bool setData(const QModelIndex& i, const QVariant& v, int role) override;
    Qt::ItemFlags flags(const QModelIndex& i) const override { return i.isValid() ? Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsEditable : Qt::NoItemFlags; }
    QHash<int,QByteArray> roleNames() const override;

    Q_INVOKABLE void appendParagraph(const QString& text, int indent=0);
    Q_INVOKABLE void appendImage(const QUrl& src, int indent=0);
    Q_INVOKABLE void convertType(int row, BlockType t);

    // optional: move support for drag&drop reordering
    bool moveRows(const QModelIndex& srcParent, int srcRow, int count,
                  const QModelIndex& dstParent, int dstRow) override;

private:
    struct Block {
        BlockType type = Paragraph;
        int indent = 0;
        QString text;
        QUrl imageUrl;
        // pointers to complex sub-objects (Kanban*, etc.) on demand
    };
    QVector<Block> m;
};
