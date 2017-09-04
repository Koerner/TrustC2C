#ifndef TRUSTDECISION_H
#define TRUSTDECISION_H

#include<QPair>
#include<QVector>
#include "average.h"
#include <QDebug>


class trustDecision
{
public:
    trustDecision();

    QPair<bool, double> calculateDecission(QList<QPair<double, int> > &reputationsBs, QPair<double, int> &trustAX, QPair<bool, double> &CarXsays, QPair<bool, double> &CarAthinks);

private:

    QPair<double, int> reputationWeightAverage(QList<QPair<double, int>> reputationsBs, QPair<double, int> trustAX); //3
    double trustWeightCalc(double reputationBX, double trustAX);

    int lastCheating(QList<double> elements);
    int numberWrongRecomendations(QList<double> elements);
};

#endif // TRUSTDECISION_H
