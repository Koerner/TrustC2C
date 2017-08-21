#ifndef TRUSTDECISION_H
#define TRUSTDECISION_H

#include<QPair>
#include<QVector>


class trustDecision
{
public:
    trustDecision();

    QPair<bool, double> calculateDecission(QVector<double> reputationsBs, QList<QList<double>> reputationRecordABs, QList<double> trustRecordAX, double CarXsays);

private:
    double reputationWeightCalc(double reputationBX, double reputationAB);
    double trustWeightCalc(double reputationBX, double trustAX);
    double reputationAverageCalc(QList<double> elements);
    double average(QList<double> elements);
};

#endif // TRUSTDECISION_H
