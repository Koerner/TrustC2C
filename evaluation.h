#ifndef EVALUATION_H
#define EVALUATION_H

#include "logdatabase.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>

#include "QDebug"


class evaluation
{
public:
    evaluation(QList<logDatabase>& logDataHandover);
    void rightDecission(int run, int firstInteraction, int lastInteraction, unsigned long int firstCar, unsigned long int lastCar);
    void showChart(int numDisplayCars);
    QMainWindow window;

    QList<double> ResultPercentageByCar;
    QList<double> APercentageByCar;
    QList<double> XPercentageByCar;
    QList<double> MaxPercentageByCar;

    void showChartOutcome(QList<double> ResultPercentageByCar1, QList<double> ResultPercentageByCar2, QList<double> ResultPercentageByCar3, QList<double> ResultPercentageByCar4, QList<double> MaxPercentageByCar, int numDisplayCars);
private:
    QList<logDatabase> *logData;
    void writeToFile(QList<double> ResultPercentageByCar, QList<double> APercentageByCar, QList<double> XPercentageByCar, QList<double> MaxPercentageByCar, QString filename);
    QList<double> average(QList<QList<double> > ListList);


};

#endif // EVALUATION_H
