#ifndef METABALLSLISTMODEL_H
#define METABALLSLISTMODEL_H

#include <QAbstractListModel>

class AbstractMetaball;

class MetaballsListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    MetaballsListModel(QObject *parent);
    ~MetaballsListModel();

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;

    AbstractMetaball *metaballAt(int row) const;

    void addMetaball(AbstractMetaball *metaball);
    QVector<AbstractMetaball *> metaballs();

private:
    QStringList m_freed_names;
    QStringList m_names;
    QVector<AbstractMetaball*> m_metaballs;
};

#endif // METABALLSLISTMODEL_H
