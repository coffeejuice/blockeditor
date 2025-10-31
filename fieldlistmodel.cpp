#include "fieldlistmodel.h"
#include <QUuid>
#include <algorithm>

FieldListModel::FieldListModel(QObject* parent) : QAbstractListModel(parent){}

int FieldListModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : m_fields.size();
}

QVariant FieldListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_fields.size()) return {};
    const auto& f = m_fields.at(index.row());
    switch (role) {
        case LabelRole:     return f.label;
        case ValueRole:     return f.value;
        case EditableRole:  return f.editable;
        case KeyRole:       return f.key;
        default:            return {};
    }
}

bool FieldListModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || index.row()<0 || index.row()>=m_fields.size()) return false;
    auto& f = m_fields[index.row()];
    bool changed = false;
    switch (role) {
        case LabelRole:    if (f.label!=value.toString())    { f.label=value.toString();  changed=true; } break;
        case ValueRole:    if (f.value!=value)               { f.value=value;             changed=true; } break;
        case EditableRole: if (f.editable!=value.toBool())   { f.editable=value.toBool(); changed=true; } break;
        case KeyRole:      if (f.key!=value.toString())      { f.key=value.toString();    changed=true; } break;
        default:           return false;
    }
    if (changed) emit dataChanged(index, index, {role});
    return changed;
}

QHash<int, QByteArray> FieldListModel::roleNames() const {
    return {
        { LabelRole,    "label" },
        { ValueRole,    "value" },
        { EditableRole, "editable" },
        { KeyRole,      "key" }
    };
}

Qt::ItemFlags FieldListModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) return Qt::NoItemFlags;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void FieldListModel::append(const QString &label, const QVariant &value, bool editable, const QString &key) {
    const int r = m_fields.size();
    beginInsertRows({}, r, r);
    Field f;
    f.label = label;
    f.value = value;
    f.editable = editable;
    f.key = key.isEmpty() ? QUuid::createUuid().toString(QUuid::WithoutBraces) : key;
    m_fields.push_back(std::move(f));
    endInsertRows();
}

void FieldListModel::insert(int row, const QString &label, const QVariant &value, bool editable, const QString &key) {
    const int r = std::clamp(row, 0, static_cast<int>(m_fields.size()));
    beginInsertRows({}, r, r);
    Field f;
    f.label = label;
    f.value = value;
    f.editable = editable;
    f.key = key.isEmpty() ? QUuid::createUuid().toString(QUuid::WithoutBraces) : key;
    m_fields.insert(m_fields.begin() + r, std::move(f));
    endInsertRows();
}

void FieldListModel::remove(int row) {
    if (row<0 || row>=m_fields.size()) return;
    beginRemoveRows({}, 0, 0);
    m_fields.removeAt(row);
    endRemoveRows();
}

void FieldListModel::clear() {
    if (m_fields.isEmpty()) return;
    beginResetModel();
    m_fields.clear();
    endResetModel();
}

bool FieldListModel::setValueAt(int row, const QVariant &value) {
    return setData(index(row), value, ValueRole);
}
