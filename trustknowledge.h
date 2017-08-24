#ifndef TRUSTKNOWLEDGE_H
#define TRUSTKNOWLEDGE_H

#include <QVector>
#include <QList>
#include "average.h"


class trustKnowledge
{
public:
    trustKnowledge();
    double trustFeedback(bool matching);
    double trustReputation(bool matching);
    QList<double> reputationFeedback(bool match, QList<double> &reputations);
    double getReputationFromBforX(QList<double> TrustValueListBX);
};

#endif // TRUSTKNOWLEDGE_H
