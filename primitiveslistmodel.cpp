#include "primitiveslistmodel.h"

PrimitivesListModel::PrimitivesListModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

QVariant PrimitivesListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_primitives.size())
        return QVariant();

    if (role == Qt::DisplayRole)
        return m_primitives.value(index.row());
}

int PrimitivesListModel::rowCount(const QModelIndex &parent) const
{
    return m_primitives.size();
}
