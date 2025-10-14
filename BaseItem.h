#ifndef BLOCKITEM_H
#define BLOCKITEM_H

#include <memory>
#include <string>

struct DocParams
{
    std::size_t index{0};
    std::size_t mesh_density{0};
    std::string number{};
    // std::string material{};

    std::string name;
    std::string mesh_elements;
    std::size_t material_id;
    std::string weight;

    std::string _sql_query_formula = "name VARCHAR(255) DEFAULT NULL, "
                         "mesh_elements SMALLINT NOT NULL DEFAULT 10, "
                         "material_id SMALLINT NOT NULL DEFAULT 1,"
                         "FOREIGN KEY (material_id) REFERENCES material(material_id) ON DELETE SET DEFAULT,"
                         "weight NUMERIC(9, 3) DEFAULT NULL";

    // Foreign key helper methods (stubs)
    static std::string material(int16_t material_id)
    {
        // TODO: Call corresponding class or service to resolve 'material_name' by 'material_id'.
        return {};
    }

    DocParams() = default;
};

struct BlockParams
{
    std::shared_ptr<std::string> press;
    std::shared_ptr<std::string> die;
    std::shared_ptr<std::string> speed_upset;
    std::shared_ptr<std::string> speed_draw;

    BlockParams()
        : press(nullptr)
        , die(nullptr)
        , speed_upset(nullptr)
        , speed_draw(nullptr)
    {}
};

using DocParamsPtr = std::shared_ptr<DocParams>;

// Base item representing a block in the BlocksListModel
class BaseItem {
protected:
    int m_block_index { -1 };         // unique per block within document
    DocParamsPtr m_doc;
    BlockParams m_block;
    bool m_dirty = true;

    QVariantMap m_payload;            // used by the model to expose data to QML

    // Protected constructors used by derived classes to initialize m_doc
    explicit BaseItem(DocParamsPtr doc) : m_doc(std::move(doc)) {}
    explicit BaseItem(BlockParams block) : m_block(std::move(block)) {}
public:
    BaseItem() = default;
    virtual ~BaseItem() = default;

    // m_block_index
    [[nodiscard]] int blockIndex() const { return m_block_index; }
    void setBlockIndex(const int index) { m_block_index = index; }

    // m_doc
    [[nodiscard]] DocParamsPtr docParamsPtr() const { return m_doc; }
    void setDocParamsPtr(DocParamsPtr docPtr) {m_doc = std::move(docPtr); }

    // m_block
    [[nodiscard]] BlockParams blockParams() const { return m_block; }
    void setBlockParams(BlockParams blockPtr) { m_block = std::move(blockPtr); }
};

// DocumentBeginItem: can initialize the shared pointer to m_doc_number
class DocBeginItem final : public BaseItem {
public:
    DocBeginItem() : BaseItem(std::make_shared<DocParams>()) {} // Call protected constructor of base class
};

class BlockBeginItem final : public BaseItem
{
public:
    BlockBeginItem() : BaseItem(BlockParams()) {} // Call protected constructor of base class
};

class HeatItem final : public BaseItem
{
public:
    HeatItem() = default; // Call protected constructor of base class
};

#endif // BLOCKITEM_H
