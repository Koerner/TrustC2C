#ifndef INTERACTION_H
#define INTERACTION_H

#include <random>
#include <iostream>
#include <QtDebug>
#include "randomQ.h"


class interaction
{
public:
    interaction(QVector<int> &settingsVector, randomQ& truthRandHandover, randomQ& detectRandXHandover, randomQ& honestRandHandover, randomQ &honestRandAHandover);

    void run();

private:
    randomQ *truthRand;
    randomQ *detectRandX;
    randomQ *honestRand;
    randomQ *detectRandA;

    //Enviroment
    bool truth;
    void initTruth();

    //CarX
    int probCarXDetects; //Probability that the carX knows what is true
    int probCarXHonest; //Probability that the car tells what it thinks is true

    void CarXDetectionResult();
    void CarXHonestResult();

    bool CarXKnowsTruth;
    bool CarXHonest;

    //CarA
    int probCarADetects; //Probability that the carA knows what is true
    void CarADetectionResult();
    bool CarAKnowsTruth;
    void determineTrustForXFromA();
};

#endif // INTERACTION_H
