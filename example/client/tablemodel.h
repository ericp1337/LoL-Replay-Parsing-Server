#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QObject>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QFile>
#include <QDebug>
#include <QPixmap>

class TableModel : public QStandardItemModel
{
public:
    QVariant data(const QModelIndex &idx, int role) const;
};

#endif // TABLEMODEL_H
