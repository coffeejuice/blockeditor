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


struct RootForgingRequirementsDie : B {

    const int type_id = 8;
    const int parent_type_id = 24;

    RootForgingRequirementsDie()
    {
         _is_obsolete = false;
        _library_name = "Die";
        _process_name = "Die";
        _labels = "";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "accumulate";
        _sql_query_formula = "";
    }
};

struct RootForgingRequirementsDieAssemblyType19 : B {

    const int type_id = 19;
    const int parent_type_id = 8;


    int32_t die_assembly_id;

    // Foreign key helper methods (stubs)
    std::string die_assembly_flat(std::string id) const {
        // TODO: Call corresponding class or service to resolve 'name' by 'id'.
        return std::string();
    }

    RootForgingRequirementsDieAssemblyType19()
    {
        _is_obsolete = false;
        _library_name = "Flat (Top + Bottom)";
        _process_name = "Flat (Top + Bottom), {}";
        _labels = "Select Flat die set:";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "accumulate";
        _sql_query_formula = "\\n        die_assembly_id INT NOT NULL DEFAULT 1,\\n        FOREIGN KEY (die_assembly_id) REFERENCES die_assembly(id) ON DELETE SET DEFAULT";
    }
};

struct RootForgingRequirementsDieAssemblyType20 : B {

    const int type_id = 20;
    const int parent_type_id = 8;


    int32_t die_assembly_id;

    // Foreign key helper methods (stubs)
    std::string die_assembly_v_die(std::string id) const {
        // TODO: Call corresponding class or service to resolve 'name' by 'id'.
        return std::string();
    }

    RootForgingRequirementsDieAssemblyType20()
    {
        _is_obsolete = false;
        _library_name = "V-die (Top + Bottom)";
        _process_name = "V-die (Top + Bottom), {}";
        _labels = "Select V-die set:";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "accumulate";
        _sql_query_formula = "\\n        die_assembly_id INT NOT NULL DEFAULT 1,\\n        FOREIGN KEY (die_assembly_id) REFERENCES die_assembly(id) ON DELETE SET DEFAULT";
    }
};

struct RootForgingRequirementsDieAssemblyType21 : B {

    const int type_id = 21;
    const int parent_type_id = 8;


    int32_t die_assembly_id;

    // Foreign key helper methods (stubs)
    std::string die_assembly_rounding(std::string id) const {
        // TODO: Call corresponding class or service to resolve 'name' by 'id'.
        return std::string();
    }

    RootForgingRequirementsDieAssemblyType21()
    {
        _is_obsolete = false;
        _library_name = "Spring rounding (Top + Bottom)";
        _process_name = "Spring rounding (Top + Bottom), {}";
        _labels = "Select Rounding die set:";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "accumulate";
        _sql_query_formula = "\\n        die_assembly_id INT NOT NULL DEFAULT 1,\\n        FOREIGN KEY (die_assembly_id) REFERENCES die_assembly(id) ON DELETE SET DEFAULT";
    }
};

struct RootForgingRequirementsDieAssemblyType65 : B {

    const int type_id = 65;
    const int parent_type_id = 8;


    int32_t die_assembly_id;

    // Foreign key helper methods (stubs)
    std::string die_assembly_gfm_die(std::string id) const {
        // TODO: Call corresponding class or service to resolve 'name' by 'id'.
        return std::string();
    }

    RootForgingRequirementsDieAssemblyType65()
    {
        _is_obsolete = false;
        _library_name = "GFM dies";
        _process_name = "GFM dies, {}";
        _labels = "Select GFM die set:";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "accumulate";
        _sql_query_formula = "\\n        die_assembly_id INT NOT NULL DEFAULT 1,\\n        FOREIGN KEY (die_assembly_id) REFERENCES die_assembly(id) ON DELETE SET DEFAULT";
    }
};

struct RootForgingRequirementsDieAssemblyType22 : B {

    const int type_id = 22;
    const int parent_type_id = 8;

    RootForgingRequirementsDieAssemblyType22()
    {
        _is_obsolete = false;
        _library_name = "Custom";
        _process_name = "Custom";
        _labels = "";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "accumulate";
        _sql_query_formula = "";
    }
};

struct RootForgingRequirementsDieAssemblyTypeInstallationSide : B {

    const int type_id = 27;
    const int parent_type_id = 22;

    RootForgingRequirementsDieAssemblyTypeInstallationSide()
    {
        _is_obsolete = false;
        _library_name = "Top die";
        _process_name = "Top die";
        _labels = "";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "accumulate";
        _sql_query_formula = "";
    }
};

struct RootForgingRequirementsDieAssemblyTypeInstallationSideDieType29 : B {

    const int type_id = 29;
    const int parent_type_id = 27;

    std::size_t top_die_id;

    // Foreign key helper methods (stubs)
    static std::string die_top_flat(std::size_t id) {
        // TODO: Call corresponding class or service to resolve 'name' by 'id'.
        return {};
    }

    RootForgingRequirementsDieAssemblyTypeInstallationSideDieType29()
        : top_die_id(0)
    {
        _is_obsolete = false;
        _library_name = "Flat";
        _process_name = "Flat, {}";
        _labels = "Select Top Flat die:";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "accumulate";
        _sql_query_formula = "\\n        top_die_id INT NOT NULL DEFAULT 1,\\n        FOREIGN KEY (top_die_id) REFERENCES die(id) ON DELETE SET DEFAULT";
    }
};

struct RootForgingRequirementsDieAssemblyTypeInstallationSideDieType30 : B {

    const int type_id = 30;
    const int parent_type_id = 27;


    int32_t top_die_id;

    // Foreign key helper methods (stubs)
    std::string die_top_v_die(std::string id) const {
        // TODO: Call corresponding class or service to resolve 'name' by 'id'.
        return std::string();
    }

    RootForgingRequirementsDieAssemblyTypeInstallationSideDieType30()
        : top_die_id(0)
    {
        _is_obsolete = false;
        _library_name = "V-die";
        _process_name = "V-die, {}";
        _labels = "Select Top V-die:";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "accumulate";
        _sql_query_formula = "\\n        top_die_id INT NOT NULL DEFAULT 1,\\n        FOREIGN KEY (top_die_id) REFERENCES die(id) ON DELETE SET DEFAULT";
    }
};

struct RootForgingRequirementsDieAssemblyTypeInstallationSideDieType31 : B {

    const int type_id = 31;
    const int parent_type_id = 27;


    std::size_t top_die_id;

    // Foreign key helper methods (stubs)
    static std::string die_top_rounding(std::size_t id)
    {
        // TODO: Call corresponding class or service to resolve 'name' by 'id'.
        return {};
    }

    RootForgingRequirementsDieAssemblyTypeInstallationSideDieType31()
        : top_die_id(0)
    {
        _is_obsolete = true;
        _library_name = "Spring rounding";
        _process_name = "Spring rounding, {}";
        _labels = "Select Top Rounding die:";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "accumulate";
        _sql_query_formula = "top_die_id INT NOT NULL DEFAULT 1, "
                             "FOREIGN KEY (top_die_id) REFERENCES die(id) ON DELETE SET DEFAULT";
    }
};

struct RootForgingRequirementsDieAssemblyTypeInstallationSide28 : B {

    const int type_id = 28;
    const int parent_type_id = 22;

    RootForgingRequirementsDieAssemblyTypeInstallationSide28()
    {
        _is_obsolete = false;
        _library_name = "Bottom die";
        _process_name = "Bottom die";
        _labels = "";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "accumulate";
        _sql_query_formula = "";
    }
};

struct RootForgingRequirementsDieAssemblyTypeInstallationSideDieType : B {

    const int type_id = 32;
    const int parent_type_id = 28;

    std::size_t bottom_die_id;

    // Foreign key helper methods (stubs)
    static std::string die_bottom_flat(std::size_t id) {
        // TODO: Call corresponding class or service to resolve 'name' by 'id'.
        return {};
    }

    RootForgingRequirementsDieAssemblyTypeInstallationSideDieType()
    {
        _is_obsolete = false;
        _library_name = "Flat";
        _process_name = "Flat, {}";
        _labels = "Select Bottom Flat die:";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "accumulate";
        _sql_query_formula = "\\n        bottom_die_id INT NOT NULL DEFAULT 1,\\n        FOREIGN KEY (bottom_die_id) REFERENCES die(id) ON DELETE SET DEFAULT";
    }
};

struct RootForgingRequirementsDieAssemblyTypeInstallationSideDieType : B {

    const int type_id = 33;
    const int parent_type_id = 28;


    int32_t bottom_die_id;

    // Foreign key helper methods (stubs)
    std::string die_bottom_v_die(std::string id) const {
        // TODO: Call corresponding class or service to resolve 'name' by 'id'.
        return std::string();
    }

    RootForgingRequirementsDieAssemblyTypeInstallationSideDieType()
    {
        _is_obsolete = false;
        _library_name = "V-die";
        _process_name = "V-die, {}";
        _labels = "Select Bottom V-die:";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "accumulate";
        _sql_query_formula = "\\n        bottom_die_id INT NOT NULL DEFAULT 1,\\n        FOREIGN KEY (bottom_die_id) REFERENCES die(id) ON DELETE SET DEFAULT";
    }
};

struct RootForgingRequirementsDieAssemblyTypeInstallationSideDieType : B {

    const int type_id = 34;
    const int parent_type_id = 28;


    int32_t bottom_die_id;

    // Foreign key helper methods (stubs)
    std::string die_bottom_rounding(std::string id) const {
        // TODO: Call corresponding class or service to resolve 'name' by 'id'.
        return std::string();
    }

    RootForgingRequirementsDieAssemblyTypeInstallationSideDieType()
          _is_obsolete(true)
    {
        _library_name = "Spring rounding";
        _process_name = "Spring rounding, {}";
        _labels = "Select Bottom Rounding die:";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "accumulate";
        _sql_query_formula = "\\n        bottom_die_id INT NOT NULL DEFAULT 1,\\n        FOREIGN KEY (bottom_die_id) REFERENCES die(id) ON DELETE SET DEFAULT";
    }
};
