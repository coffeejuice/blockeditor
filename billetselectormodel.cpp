#include <QJsonObject>
// #include <QJsonDocument>
#include <QVariantMap>
// #include <QVariantList>
#include <QList>
#include <QString>
#include <QByteArray>
#include "blockslistmodel.h"
#include "BaseItem.h"

BlocksListModel::BlocksListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_types = {"documentBegin", "blockBegin", "heat", "upset", "draw", "blockBegin", "heat", "draw", "upset"};

    // Prepare shared pointers that could be shared among items
    // auto docNumber = std::make_shared<std::string>("100.0342.0");
    // auto curve = std::make_shared<std::vector<double>>();
    // std::shared_ptr<std::string> currentBlockNamePtr; // will be shared among items in the same block

    {
        auto it = std::make_unique<DocBeginItem>();
        // QVariantMap p;
        // p.insert("type", "documentBegin");
        // p.insert("image", 1);
        // p.insert("documentNumber", QString::fromStdString(*docNumber));
        // p.insert("material", "Inconel718");
        // p.insert("meshDensity", 10);
        // it->setDocumentNumberPtr(docNumber);
        // it->setBlockNamePtr(currentBlockNamePtr);
        // it->setPayload(p);
        m_blocks.append(std::move(it));
    }
    {
        auto it = std::make_unique<BlockBeginItem>();
        // it->setBlockNamePtr(currentBlockNamePtr);
        // QVariantMap p;
        // p.insert("type", "blockBegin");
        // p.insert("image", 2);
        // p.insert("press", "80MN");
        // p.insert("dies", "V-dies 320-260");
        // p.insert("feedFirst", 250);
        // p.insert("feedOther", 200);
        // p.insert("speedUpset", 20);
        // p.insert("speedDraw", 80);
        // it->setPayload(p);
        m_blocks.append(std::move(it));
        // currentBlockNamePtr = m_blocks.back()->blockNamePtr();
    }
    {
        auto it = std::make_unique<HeatItem>();
        // it->setBlockNamePtr(currentBlockNamePtr);
        // QVariantMap p;
        // p.insert("type", "heat");
        // p.insert("image", 3);
        // p.insert("timeUnits", "minutes");
        // p.insert("typeTimeTemperature", QVariantList{
        //     "preheatFurnace", 0, 1000,
        //     "openDoor", 20, 1000,
        //     "openDoor", 40, 700,
        //     "heating", 60, 1000,
        //     "heating", 360, 1000,
        //     "soaking", 400, 1000
        // });
        // it->setPayload(p);
        m_blocks.append(std::move(it));
    }
    {
        auto it = std::make_unique<BaseItem>();
        // it->setBlockNamePtr(currentBlockNamePtr);
        // QVariantMap p;
        // p.insert("type", "upset");
        // p.insert("image", 4);
        // p.insert("operations", "(1000)->800->700");
        // it->setPayload(p);
        m_blocks.append(std::move(it));
    }
    {
        auto it = std::make_unique<BaseItem>();
        // it->setBlockNamePtr(currentBlockNamePtr);
        // QVariantMap p;
        // p.insert("type", "draw");
        // p.insert("image", 5);
        // p.insert("operations", "(600)->550->(90)580->(90)535->(90)560->(45)600->(45)610");
        // it->setPayload(p);
        m_blocks.append(std::move(it));
    }
    {
        auto it = std::make_unique<BlockBeginItem>();
        // it->setBlockNamePtr(currentBlockNamePtr);
        // QVariantMap p;
        // p.insert("type", "blockBegin");
        // p.insert("image", 6);
        // p.insert("press", "40MN");
        // p.insert("dies", "V-dies 280-240");
        // p.insert("feedFirst", 200);
        // p.insert("feedOther", 100);
        // p.insert("speedUpset", 20);
        // p.insert("speedDraw", 80);
        // it->setPayload(p);
        m_blocks.append(std::move(it));
        // currentBlockNamePtr = m_blocks.back()->blockNamePtr();
    }
    {
        auto it = std::make_unique<HeatItem>();
        // it->setBlockNamePtr(currentBlockNamePtr);
        // QVariantMap p;
        // p.insert("type", "heat");
        // p.insert("image", 7);
        // p.insert("timeUnits", "minutes");
        // p.insert("typeTimeTemperature", QVariantList{
        //     "preheatFurnace", 0, 1000,
        //     "openDoor", 20, 1000,
        //     "openDoor", 40, 700,
        //     "heating", 60, 1000,
        //     "heating", 360, 1000,
        //     "soaking", 400, 1000
        // });
        // it->setPayload(p);
        m_blocks.append(std::move(it));
    }
    {
        auto it = std::make_unique<BaseItem>();
        // it->setBlockNamePtr(currentBlockNamePtr);
        // QVariantMap p;
        // p.insert("type", "draw");
        // p.insert("image", 8);
        // p.insert("operations", "(500)->450->(90)480->(90)435->(90)460->(45)500->(45)510");
        // it->setPayload(p);
        m_blocks.append(std::move(it));
    }
    {
        auto it = std::make_unique<BaseItem>();
        // it->setBlockNamePtr(currentBlockNamePtr);
        // QVariantMap p;
        // p.insert("type", "upset");
        // p.insert("image", 9);
        // p.insert("operations", "(1400)->1100->900");
        // it->setPayload(p);
        m_blocks.append(std::move(it));
    }
}

int BlocksListModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return static_cast<int>(m_types.size());
}

QVariant BlocksListModel::data(const QModelIndex &index, int role) const {
    if (!checkIndex(index, CheckIndexOption::IndexIsValid))
        return QVariant();

    if (!index.isValid())
        return QVariant();

    const int row = index.row();

    switch (role) {
        case BlockTypeRole:     return m_types[row];
        // case BlockContentRole:  return m_blocks[row]->toVariantMap();
    }
    return QVariant();
}

QHash<int, QByteArray> BlocksListModel::roleNames() const {
    QHash<int, QByteArray> mapping {
        {BlocksRoles::BlockTypeRole, "blockType"},
        {BlocksRoles::BlockContentRole, "blockContent"},
    };
    return mapping;
}

Qt::ItemFlags BlocksListModel::flags(const QModelIndex &index) const {
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

bool BlocksListModel::setData(const QModelIndex &index, const QVariant &value, int role) {

    // if (!index.isValid()) return false;
    if (role != Qt::EditRole) return false;

    const int row = index.row();
    if (row < 0 || row >= m_types.size()) return false;
    if (!value.canConvert<QVariantMap>()) return false;

    const QVariantMap payload = value.toMap();

    // Expect a composite object: { "type": ..., "content": ... }
    if (!payload.contains("type")) return false;
    if (!payload.contains("content")) return false;

    m_types[row] = std::move(payload.value("type").toString());
    {
        auto map = payload.value("content").toMap();
        // m_blocks[row]->setPayload(map);
    }

    emit dataChanged(index, index);

    updateDocumentCache();

    return true;
}

bool BlocksListModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationRow) {
    int newDestination = destinationRow;
    if (sourceRow < destinationRow)
        newDestination++;

    beginMoveRows(sourceParent, sourceRow, sourceRow, destinationParent, newDestination);

    m_types.move(sourceRow, destinationRow);
    m_blocks.move(sourceRow, destinationRow);

    endMoveRows();

    updateDocumentCache();

    return true;
}

bool BlocksListModel::insertRows(int row, int count, const QModelIndex &parent) {
    beginInsertRows(parent, row, row + count - 1);

    for (int i = 0; i < count; ++i) {
        m_types.insert(row, QString{});
        auto newItem = std::make_unique<BaseItem>();
        if (row > 0) {
            // newItem->setBlockNamePtr(m_blocks[row - 1]->blockNamePtr());
        }
        m_blocks.insert(row, std::move(newItem));
        if (row + 1 < m_blocks.size()) {
            // m_blocks[row + 1]->setBlockNamePtr(m_blocks[row]->blockNamePtr());
        }
        ++row;
    }

    endInsertRows();

    updateDocumentCache();

    return true;
}

bool BlocksListModel::removeRows(int row, int count, const QModelIndex &parent) {
    beginRemoveRows(parent, row, row + count - 1);

    for (int i = 0; i < count; ++i) {
        m_types.removeAt(row);
        m_blocks.removeAt(row);
    }

    endRemoveRows();

    updateDocumentCache();

    return true;
}

bool BlocksListModel::appendRow(int rowNumber) {
    beginInsertRows(QModelIndex(), rowNumber, rowNumber);

    m_types.append(QString{});

    auto newItem = std::make_unique<BaseItem>();
    if (!m_blocks.isEmpty()) {
        // newItem->setBlockNamePtr(m_blocks.back()->blockNamePtr());
    }
    m_blocks.append(std::move(newItem));

    endInsertRows();

    updateDocumentCache();

    return true;
}

bool BlocksListModel::clearModel(int rowNumber) {
    const int previousRow = rowNumber - 1;
    beginRemoveRows(QModelIndex(), 0, previousRow);

    m_types.remove(0, previousRow);
    m_blocks.remove(0, previousRow);

    endRemoveRows();

    updateDocumentCache();

    return true;
}


struct B {
    std::string _library_name;
    std::string _process_name;
    std::string _labels;
    bool _is_obsolete;
    std::string _deformation_type;
    std::string _speed_column_name;
    std::string _trigger;
    std::string _sql_query_formula;
};

struct Root : B {

    const int type_id = 1;
    const int parent_type_id = 0;


    std::string name;
    int16_t mesh_elements;
    int16_t material_id;
    double weight;

    Root() {
        _is_obsolete = false;
        _library_name = "Simulation|"
                        "Mesh|"
                        "Material|"
                        "Weight";
        _process_name = "{} simulation|"
                        "Mesh: {} elements across width|"
                        "Material: {}|"
                        "Weight: {} kg";
        _labels = "Name:|"
                  "Elements across width [pcs]:|"
                  "Material:|"
                  "Weight [kg]:";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "initialize";
        _sql_query_formula = "name VARCHAR(255) DEFAULT NULL, "
                             "mesh_elements SMALLINT NOT NULL DEFAULT 10, "
                             "material_id SMALLINT NOT NULL DEFAULT 1,"
                             "FOREIGN KEY (material_id) REFERENCES material(material_id) ON DELETE SET DEFAULT,"
                             "weight NUMERIC(9, 3) DEFAULT NULL";
    }

    // Foreign key helper methods (stubs)
    [[nodiscard]] std::string material(int16_t material_id) const
    {
        // TODO: Call corresponding class or service to resolve 'material_name' by 'material_id'.
        return std::string();
    }
};

struct RootBilletGeometryShape68 : B {

    const int type_id = 68;
    const int parent_type_id = 7;


    std::string diameter;

    RootBilletGeometryShape68()
    : diameter("0")
    {
        _library_name = "◯ - round D [mm]";
        _process_name = "◯ - round D {} mm";
        _labels = "Diameter of billet [mm]:";
        _sql_query_formula = "diameter NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct RootBilletGeometryShape69 : B {

    const int type_id = 69;
    const int parent_type_id = 7;


    std::string diameter;
    std::string tail_radius;

    RootBilletGeometryShape69()
    : diameter("0")
    , tail_radius("0")
    {
        _library_name = "◯ - round D, Tail edge radius [mm]";
        _process_name = "◯ - round D {}, tail edge radius {} mm";
        _labels = "Diameter of billet [mm]|Tail radius [mm]:";
        _sql_query_formula = "diameter NUMERIC(9, 3) DEFAULT NULL, "
                             "tail_radius NUMERIC(9, 3) DEFAULT NULL";
    }

};

struct RootBilletGeometryShape : B {

    const int type_id = 70;
    const int parent_type_id = 7;


    std::string diameter;
    std::string tail_chamfer;

    RootBilletGeometryShape()
    : diameter("0")
    , tail_chamfer("0")
    {
        _library_name = "◯ - round D, Tail chamfer x 45° [mm]";
        _process_name = "◯ - round D {} mm, tail edge chamfer {} mm x 45°";
        _labels = "Diameter of billet [mm]|Tail chamfer [mm]:";
        _sql_query_formula = "diameter NUMERIC(9, 3) DEFAULT NULL,"
                             "tail_chamfer NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct RootBilletGeometryShape71 : B {
    const int type_id = 71;
    const int parent_type_id = 7;

    std::string length_to_diameter_ratio;

    RootBilletGeometryShape71()
        : length_to_diameter_ratio("0")
    {
        _library_name = "◯ - round L/D ratio";
        _process_name = "◯ - round L/D ratio = {}";
        _labels = "Length/Diameter billet ratio:";
        _sql_query_formula = "length_to_diameter_ratio REAL DEFAULT NULL";
    }
};

struct RootBilletGeometryShape72 : B {
    const int type_id = 72;
    const int parent_type_id = 7;

    std::string side_of_square;

    RootBilletGeometryShape72()
        : side_of_square("0")
    {
        _library_name = "⬜ - square H [mm]";
        _process_name = "⬜ - square H {} mm";
        _labels = "Height of ⬜ billet section [mm]:";
        _sql_query_formula = "side_of_square NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct RootBilletGeometryShape73 : B {
    const int type_id = 73;
    const int parent_type_id = 7;

    std::string side_of_square;
    std::string diagonal;

    RootBilletGeometryShape73()
    : side_of_square("0")
    , diagonal("0")
    {
        _library_name = "⬜ - square H, Diagonal [mm]";
        _process_name = "⬜ - square H {}, diagonal {} mm";
        _labels = "Height of ⬜ billet section [mm]|Diagonal of section[mm]:";
        _sql_query_formula = "side_of_square NUMERIC(9, 3) DEFAULT NULL, "
                             "diagonal NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct RootBilletGeometryShape74 : B {
    const int type_id = 74;
    const int parent_type_id = 7;

    std::string length_to_side_ratio;

    RootBilletGeometryShape74()
    : length_to_side_ratio("0")
    {
        _library_name = "⬜ - square L/H ratio";
        _process_name = "⬜ - square L/H ratio = {}";
        _labels = "Length/Height billet ratio:";
        _sql_query_formula = "length_to_side_ratio REAL DEFAULT NULL";
    }
};

struct RootBilletGeometryShape75 : B {
    const int type_id = 75;
    const int parent_type_id = 7;

    std::string height;
    std::string width;

    RootBilletGeometryShape75()
    : height("0")
    , width("0")
    {
        _library_name = "▯ - rectangle H x W [mm]";
        _process_name = "▯ - rectangle H x W = {} x {} mm";
        _labels = "Height of billet section [mm]:|Width of billet section [mm]:";
        _sql_query_formula = "height NUMERIC(9, 3) DEFAULT NULL, "
                             "width NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct RootBilletGeometryShape76 : B {
    const int type_id = 76;
    const int parent_type_id = 7;

    std::string height_to_width_ratio;
    std::string length_to_thickness_ratio;

    RootBilletGeometryShape76()
    : height_to_width_ratio("0")
    , length_to_thickness_ratio("0")
    {
        _library_name = "▯ - rectangle H/W ratio, L/Thickness ratio";
        _process_name = "▯ - rectangle H/W ratio = {}, L/Thickness ratio = {}";
        _labels = "Height/Width of billet section:|Length/Thickness:";
        _sql_query_formula = "height_to_width_ratio REAL DEFAULT NULL, "
                             "length_to_thickness_ratio REAL DEFAULT NULL";
    }
};

struct RootBilletGeometryShape77 : B {
    const int type_id = 77;
    const int parent_type_id = 7;

    std::string height;
    std::string width;
    std::string diagonal;

    RootBilletGeometryShape77()
    : height("0")
    , width("0")
    , diagonal("0")
    {
        _library_name = "▯ - rectangle H x W, Diagonal [mm]";
        _process_name = "▯ - rectangle H x W = {} x {}, diagonal {} mm";
        _labels = "Height of billet section [mm]:|Width of billet section [mm]:|Diagonal of section [mm]:";
        _sql_query_formula = "height NUMERIC(9, 3) DEFAULT NULL, "
                             "width NUMERIC(9, 3) DEFAULT NULL, "
                             "diagonal NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct RootBilletGeometryShape78 : B {
    const int type_id = 78;
    const int parent_type_id = 7;

    std::string height;
    std::string width;
    std::string diagonal_1;
    std::string diagonal_2;

    RootBilletGeometryShape78()
    : height("0")
    , width("0")
    , diagonal_1("0")
    , diagonal_2("0")
    {
        _library_name = "▯ - rectangle H x W, Two diagonals [mm]";
        _process_name = "▯ - rectangle H x W = {} x {}, diagonal #1 = {}, diagonal #2 = {} mm";
        _labels = "Height of billet section [mm]:|Width of billet section [mm]:|Diagonal #1 of section [mm]:|Diagonal #2 of section [mm]:";
        _sql_query_formula = "height NUMERIC(9, 3) DEFAULT NULL, "
                             "width NUMERIC(9, 3) DEFAULT NULL, "
                             "diagonal_1 NUMERIC(9, 3) DEFAULT NULL, "
                             "diagonal_2 NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct RootBilletGeometryShape79 : B {
    const int type_id = 79;
    const int parent_type_id = 7;

    std::string height;

    RootBilletGeometryShape79()
    : height("0")
    {
        _library_name = "⯃ - octagon H [mm]";
        _process_name = "⯃ - octagon H = {} mm";
        _labels = "Height of octagon section [mm]";
        _sql_query_formula = "height NUMERIC(9, 3) DEFAULT NULL";
    }
};
