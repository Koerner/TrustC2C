#ifndef GUITABLE_H
#define GUITABLE_H

#include <QtWidgets/QWidget>
#include <QtGui>

#include "database.h"
#include "logdatabase.h"
#include <QLineEdit>
#include <QPushButton>
#include <QtWidgets/QTableView>

#include "guiChart.h"
#include "guitableforchart.h"


#include <QtCharts/QChart>



class guiChart : public QWidget
{

    Q_OBJECT

public:
    guiChart(logDatabase &logDatabaseHandover, database &databaseHandover, int CarIDHandover, int firstHandover, int lastHandover, QWidget *parent = 0);

private:
    QPushButton *carButton;
    QLineEdit *carEdit;
    guiTableForChart *model;
    logDatabase *logData;
    database *data;
    int CarID;
    int first;
    int last;

public slots:
    void updateTable();

signals:
    //void valueChanged(int newValue);
};

#endif // GUITABLE_H
