#include "guitableforchart.h"
#include <QtCore/QVector>
#include <QtCore/QTime>
#include <QtCore/QRect>
#include <QtGui/QColor>
#include "trustDecision.h"

guiTableForChart::guiTableForChart(logDatabase &logDatabaseHandover, database &databaseHandover, QObject *parent) :
    QAbstractTableModel(parent)

{
    m_columnCount = 9;
    m_rowCount = logDatabaseHandover.sizeInteractionLogDatabase();

    // m_data
    for (int i = 0; i < m_rowCount; i++) {
        if(logDatabaseHandover.getInteractionLog(i).carIDs.at(0)==9)
        {
            QVector<qreal>* dataVec = new QVector<qreal>(m_columnCount);
            for (int k = 0; k < dataVec->size(); k++)
            {
                switch(k)
                {
                case 0: //x-Axis I //# interaction
                    dataVec->replace(k, i);
                    break;
                case 1: //y-Axis I //# involved interaction
                    dataVec->replace(k, (m_data.size()+1));
                    break;
                case 2: //y-Axes II //#correct decission
                    dataVec->replace(k, logDatabaseHandover.getInteractionLog(i).correctDecission);
                    break;
                case 3: //y-Axes III //certainty decisssion
                    dataVec->replace(k, logDatabaseHandover.getInteractionLog(i).descissionResult.second);
                    break;
                case 4: //y-Axes IV //own prediction
                    dataVec->replace(k, logDatabaseHandover.getInteractionLog(i).carAprediction.first==logDatabaseHandover.getInteractionLog(i).truth);
                    break;
                case 5: //y-Axes V //own prediction certainty
                    dataVec->replace(k, logDatabaseHandover.getInteractionLog(i).carAprediction.second);

                    break;
                case 6: //y-Axes VI //CarX says state
                    dataVec->replace(k, logDatabaseHandover.getInteractionLog(i).carXsays.first==logDatabaseHandover.getInteractionLog(i).truth);
                    break;
                case 7: //y-Axes VII //CarX certainty
                    dataVec->replace(k, logDatabaseHandover.getInteractionLog(i).carXsays.second);
                    break;
                case 8: //y-Axes VIII //CarX reputation
                    trustDecision trusttemp;
                    QPair<double, int> temp = trusttemp.reputationWeightAverage(logDatabaseHandover.getInteractionLog(i).reputations, qMakePair(0,0)); //3
                    dataVec->replace(k, temp.first);
                    break;
                }
            }

            m_data.append(dataVec);
        }
    }
}

int guiTableForChart::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_data.count();
}

int guiTableForChart::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_columnCount;
}

QVariant guiTableForChart::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch(section)
        {
        case 0:
            return "#";
            break;
        case 1:
            return "#(A)";
            break;
        case 2:
            return "Correct";
            break;
        case 3:
            return "%(cert)";
            break;
        case 4:
            return "state(A)";
            break;
        case 5:
            return "cert(A)";
            break;
        case 6:
            return "state(X)";
            break;
        case 7:
            return "cert(X)";
            break;
        case 8:
            return "Rep(B->X)";
            break;
        }

    } else {
        return QString("%1").arg(section + 1);
    }
}

QVariant guiTableForChart::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        return m_data[index.row()]->at(index.column());
    } else if (role == Qt::EditRole) {
        return m_data[index.row()]->at(index.column());
    } else if (role == Qt::BackgroundRole) {
        foreach (QRect rect, m_mapping) {
            if (rect.contains(index.column(), index.row()))
                return QColor(m_mapping.key(rect));
        }
        // cell not mapped return white color
        return QColor(Qt::white);
    }
    return QVariant();
}

bool guiTableForChart::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        m_data[index.row()]->replace(index.column(), value.toDouble());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags guiTableForChart::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

void guiTableForChart::addMapping(QString color, QRect area)
{
    m_mapping.insertMulti(color, area);
}
