#ifndef BLOCKITEM_H
#define BLOCKITEM_H

#include <memory>
#include <string>
#include <vector>
#include <QVariantMap>
#include <QString>

// Base item representing a block in the BlocksListModel
class BlockItem {
private:
    int m_doc_index { -1 };           // unique per document
    int m_block_index { -1 };         // unique per block within document
    std::shared_ptr<std::string> m_doc_number; // shared between items of the same document
    std::shared_ptr<std::vector<double>> m_block_curve; // shared curve data per block
    std::string m_block_type;         // e.g. "documentBegin", "blockBegin", "heat"
    std::shared_ptr<std::string> m_block_name; // shared name across related items

protected:
    QVariantMap m_payload;            // used by model to expose data to QML
    // Allow derived classes (DocumentBeginItem) to initialize the doc number pointer
    void assignDocNumberPtr(const std::shared_ptr<std::string> &ptr) { m_doc_number = ptr; }
    // Allow derived classes to initialize/replace the block name pointer
    void assignBlockNamePtr(const std::shared_ptr<std::string> &ptr) { m_block_name = ptr; }

public:
    BlockItem() = default;

    virtual ~BlockItem() = default;

    // Accessors (getters for all variables)
    int docIndex() const { return m_doc_index; }
    int blockIndex() const { return m_block_index; }
    const std::shared_ptr<std::string>& docNumberPtr() const { return m_doc_number; }
    const std::shared_ptr<std::vector<double>>& blockCurvePtr() const { return m_block_curve; }
    const std::string& blockType() const { return m_block_type; }
    const std::shared_ptr<std::string>& blockNamePtr() const { return m_block_name; }

    // Public setter to link/share the block name pointer from model code
    void setBlockNamePtr(const std::shared_ptr<std::string> &ptr) { m_block_name = ptr; }

    // Helper to expose to QVariant
    virtual QVariantMap toVariantMap() const { return m_payload; }
    void setPayload(const QVariantMap &map) { m_payload = map; }
};

// DocumentBeginItem: can initialize the shared pointer to m_doc_number
class DocumentBeginItem : public BlockItem {
public:

    // Setter to initialize/replace the shared pointer for document number
    void setDocumentNumberPtr(const std::shared_ptr<std::string> &ptr) {
        // Write into the base's private member via protected helper
        assignDocNumberPtr(ptr);
        if (ptr) {
            m_payload.insert("documentNumber", QString::fromStdString(*ptr));
        } else {
            m_payload.remove("documentNumber");
        }
    }

    // Getter to obtain the shared pointer to the document number
    std::shared_ptr<std::string> documentNumberPtr() const {
        return BlockItem::docNumberPtr();
    }
};

// BlockBeginItem: shares the doc number pointer but cannot initialize it here
class BlockBeginItem : public BlockItem {
public:
    // Setter must set a string value (not a pointer) for block name
    void setBlockName(const std::string &name) {
        if (!blockNamePtr()) {
            auto p = std::make_shared<std::string>(name);
            assignBlockNamePtr(p);
        } else {
            *blockNamePtr() = name;
        }
        m_payload.insert("blockName", QString::fromStdString(*blockNamePtr()));
    }
};

// HeatItem: shares the doc number pointer but cannot initialize it here
class HeatItem : public BlockItem {
public:
};

#endif // BLOCKITEM_H
