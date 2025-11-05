#pragma once
#ifndef CARDMODEL_H
#define CARDMODEL_H

#include <QAbstractListModel>
#include <QQmlEngine>
#include "cellmodel.h"

class CardModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("CardModel is declared in C++")

public:
    class BaseCard;
    QVector<BaseCard> m_cards;

public:
    enum Roles {CardTypeRole=Qt::UserRole+1, CellModelRole};
    enum CardType { IMAGE=CellModelRole+1, GRAPHIC, PROCESS, HEAT, FORMING, UPSET, DRAW};
    Q_ENUM(CardType);

    CardModel();

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QHash<int, QByteArray> roleNames() const override;

public:

    // ==============================================================================
    class BaseCard {
        enum CardType m_type;
        CellModel* m_cell_model = nullptr;
    protected:
        void setCardType(const enum CardType type);
        void setCellModel(CellModel* m);
    public:
        enum CardType cardType() const;
        virtual CellModel* cellModel() const;
    };

    class Image : public BaseCard {
    public:
        Image();
    };

    class Graphic : public BaseCard {
    public:
        Graphic();
    };

    class Process : public BaseCard {
    public:
        Process();
    };

    class Heat : public BaseCard {
    public:
        Heat();
    };

    class Forming : public BaseCard {
    public:
        Forming();
    };

    class Upset : public BaseCard {
    public:
        Upset();
    };

    class Draw : public BaseCard {
    public:
        Draw();
    };

};

#endif // CARDMODEL_H
