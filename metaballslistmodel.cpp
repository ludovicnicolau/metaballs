#include "metaballslistmodel.h"

#include "abstractmetaball.h"

MetaballsListModel::MetaballsListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

MetaballsListModel::~MetaballsListModel()
{
    for (int i = 0;i < m_metaballs.size();++i)
        delete m_metaballs[i];
}

int MetaballsListModel::rowCount(const QModelIndex &) const
{
    return m_names.size();
}

QVariant MetaballsListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        return m_names[index.row()];
    }

    return QVariant();
}

bool MetaballsListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (parent.isValid())
        return false;

    if (row >= m_metaballs.size() || row + count <= 0)
        return false;

    int beginRow = qMax(0, row);
    int endRow = qMin(row + count - 1, m_metaballs.size());

    beginRemoveRows(parent, beginRow, endRow);
    int i = beginRow;
    while (i <= endRow)
    {
        m_freed_names.push_back(m_names[i]);
        m_names.removeAt(i);
        delete m_metaballs[i];
        m_metaballs.removeAt(i);
        ++i;
    }
    endRemoveRows();
    return true;
}

AbstractMetaball *MetaballsListModel::metaballAt(int row) const
{
    if (row >= m_metaballs.size() || row < 0)
        return nullptr;
    return m_metaballs[row];
}

void MetaballsListModel::addMetaball(AbstractMetaball *metaball)
{    
    QString name;
    if (m_freed_names.isEmpty())
        name = "metaball " + QString::number(m_metaballs.size() + 1);
    else
    {
        name = m_freed_names[0];
        m_freed_names.pop_front();
    }

    int row = m_metaballs.size();
    beginInsertRows(QModelIndex(), row, row);
    m_names.push_back(name);
    m_metaballs.push_back(metaball);
    endInsertRows();
}

QVector<AbstractMetaball *> MetaballsListModel::metaballs()
{
    return m_metaballs;
}
