#ifndef LOGMODEL_H
#define LOGMODEL_H


#include <QAbstractTableModel>
#include <QDebug>
#include <QBrush>
#include <QFont>
#include "database.h"

class LogModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    LogModel(QObject *parent, database &databaseHandover, int rowsHandover);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
    database *databasePointer;
    int rows;
};

#endif // LOGMODEL_H
