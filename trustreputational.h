#ifndef TRUSTREPUTATIONAL_H
#define TRUSTREPUTATIONAL_H

#include <QPair>
#include <QList>
#include <QDebug>

class trustReputational
{
public:
    trustReputational();

    static QPair<double, int> combine(QPair<double, int>own, QPair<double, int>foreign); //5
    static QPair<double, int> mergeTrust(QList<QPair<bool, double>>trustList); //6
    static QPair<double, int> mergeReputation(QList<QPair<bool, double>>reputationList); //4

    static double mergeReputationD(QList<QPair<bool, double>>reputationList); //4
    static double mergeTrustD(QList<QPair<bool, double>>trustList); //6

};

#endif // TRUSTREPUTATIONAL_H
