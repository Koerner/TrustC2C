#ifndef TRUSTKNOWLEDGE_H
#define TRUSTKNOWLEDGE_H

#include <QVector>
#include <QList>


class trustKnowledge
{
public:
    trustKnowledge();
    double trustFeedback(bool matching);
    double trustReputation(bool matching);
    QVector<double> reputationFeedback(bool match, QVector<double> reputations);
    double getReputationFromBforX(QList<double> TrustValueListBX);
    double average(QList<double> elements);
};

#endif // TRUSTKNOWLEDGE_H
