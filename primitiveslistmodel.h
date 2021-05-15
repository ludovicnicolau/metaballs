#ifndef PRIMITIVESLISTMODEL_H
#define PRIMITIVESLISTMODEL_H

#include <QAbstractListModel>
#include <QVector>

#include "abstractmetaball.h"

class PrimitivesListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    PrimitivesListModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent) const override;

private:
    QVector<AbstractMetaball*> m_primitives;

};

#endif // PRIMITIVESLISTMODEL_H
