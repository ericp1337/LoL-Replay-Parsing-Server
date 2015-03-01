#include "tablemodel.h"

/*
 * Caution: this is currently hard crashing the program and locking anything that tries to access the process.
 */

QVariant TableModel::data(const QModelIndex &idx, int role) const
{
    if(idx.row() == 0 || idx.row() == 6) {
        return QStandardItemModel::data(idx, role);
    }

    if(idx.column() == 6 && (role == Qt::DisplayRole || Qt::DecorationRole || Qt::SizeHintRole)) {

        if(role == Qt::DisplayRole) {
            return QString();
        }

        //QImage image(imgFile);

        QPixmap pixmap/*(imgFile)*/;
        if(role == Qt::DecorationRole) {
            //return pixmap;
            qDebug() << idx.data();
            qDebug() << "Qt::DecorationRole() called \n";
            return QPixmap();
        }

        if(role == Qt::SizeHintRole) {
            return pixmap.size();
        }
    }

    return QStandardItemModel::data(idx, role);
}
