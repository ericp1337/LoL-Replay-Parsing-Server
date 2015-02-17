#include "tablemodel.h"

QVariant TableModel::data(const QModelIndex &idx, int role) const
{
    if(idx.column() == 6 && (role == Qt::DecorationRole || role == Qt::SizeHintRole)) {
        QString imgFile = idx.data().toString();
        qDebug() << imgFile;

        if(role == Qt::DisplayRole) {
            return QString();
        }

        QImage image(imgFile);

        QPixmap pixmap(imgFile);
        if(role == Qt::DecorationRole) {
            return pixmap;
        }

        if(role == Qt::SizeHintRole) {
            return pixmap.size();
        }
    }

    return QStandardItemModel::data(idx, role);
}
