#include "logModel.h"

LogModel::LogModel(QObject *parent, database &databaseHandover, int rowsHandover)
    :QAbstractTableModel(parent)
{
    databasePointer = &databaseHandover;
    rows = rowsHandover;
}

int LogModel::rowCount(const QModelIndex & /*parent*/) const
{
   return rows;
}

int LogModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 5;
}

QVariant LogModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    switch(col){

    case 0 :
      return databasePointer->interactionLogList.at(row).truth;
    case 1 :
        return databasePointer->interactionLogList.at(row).xSays;
    case 2 :
        return databasePointer->interactionLogList.at(row).descissionResult.first;
    case 3 :
        return databasePointer->interactionLogList.at(row).success;
    case 4:
        return databasePointer->interactionLogList.at(row).descissionResult.second;
    };






            /*
    int row = index.row();
        int col = index.column();
        // generate a log message when this method gets called
        qDebug() << QString("row %1, col%2, role %3")
                .arg(row).arg(col).arg(role);

    switch(role){
        case Qt::DisplayRole:
            if (row == 0 && col == 1) return QString("<--left");
            if (row == 1 && col == 1) return QString("right-->");

            return QString("Row%1, Column%2")
                    .arg(row + 1)
                    .arg(col +1);
            break;
        case Qt::FontRole:
            if (row == 0 && col == 0) //change font only for cell(0,0)
            {
                QFont boldFont;
                boldFont.setBold(true);
                return boldFont;
            }
            break;
        case Qt::BackgroundRole:

            if (row == 1 && col == 2)  //change background only for cell(1,2)
            {
                QBrush redBackground(Qt::red);
                return redBackground;
            }
            break;
        case Qt::TextAlignmentRole:

            if (row == 1 && col == 1) //change text alignment only for cell(1,1)
            {
                return Qt::AlignRight + Qt::AlignVCenter;
            }
            break;
        case Qt::CheckStateRole:

            if (row == 1 && col == 0) //add a checkbox to cell(1,0)
            {
                return Qt::Checked;
            }
        }

        return QVariant();
                */
}


