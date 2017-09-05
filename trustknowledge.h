#ifndef TRUSTKNOWLEDGE_H
#define TRUSTKNOWLEDGE_H

#include <QVector>
#include <QList>
#include <QDebug>
#include "average.h"


class trustKnowledge
{
public:
    trustKnowledge();
    static QPair<bool, double> trustFeedback(bool matching, double sentCertainty);
    double trustReputation(bool matching);
    QList<QPair<bool, double>> reputationFeedback(bool match, double certaintyAobservation, QList<QPair<double, int> > &reputations);
    double getReputationFromBforX(QList<double> TrustValueListBX);
};

#endif // TRUSTKNOWLEDGE_H
