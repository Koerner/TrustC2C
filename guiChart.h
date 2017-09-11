#ifndef GUITABLE_H
#define GUITABLE_H

#include <QtWidgets/QWidget>

#include "database.h"
#include "logdatabase.h"


class guiChart : public QWidget
{

    Q_OBJECT

public:
    guiChart(logDatabase &logDatabaseHandover, database &databaseHandover, int CarID, int first, int last, QWidget *parent = 0);
};

#endif // GUITABLE_H
