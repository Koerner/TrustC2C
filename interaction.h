#ifndef INTERACTION_H
#define INTERACTION_H

#include <random>
#include <iostream>
#include <QtDebug>
#include "randomQ.h"


class interaction
{
public:
    interaction(QVector<int> &settingsVector, randomQ& truthRandHandover, randomQ& detectRandHandover, randomQ& honestRandHandover);

    void run();

private:
    randomQ *truthRand;
    randomQ *detectRand;
    randomQ *honestRand;

    int probCarDetects; //Probability that the car knows what is true
    int probCarHonest; //Probability that the car tells what it thinks is true

    bool CarKnowsTruth;
    bool truth;
    bool CarHonest;

    void initTruth();
    void CarDetectionResult();
    void CarHonestResult();
};

#endif // INTERACTION_H
