#ifndef INTERACTION_H
#define INTERACTION_H

#include <random>
#include <iostream>
#include <QtDebug>
#include "randomQ.h"
#include "database.h"
#include "structs.h"
#include "trustknowledge.h"
#include "trustDecision.h"


class interaction
{
public:
    interaction(settingsGUI instanceSettingsHandover, randStruct &randCollectionHandover, database &dataHandover);

    void run();

private:
    randomQ *truthRand;
    randomQ *detectRandX;
    randomQ *honestRand;
    randomQ *detectRandA;

    database *data;
    settingsGUI instanceSettings;
    randStruct *randCollection;

    QVector<unsigned long int> carIDs;

    //Enviroment
    bool truth;
    void initTruth();
    void storeInteractionHandler();

    //CarX


    void CarXDetectionResult();
    void CarXHonestResult();

    bool CarXKnowsTruth;
    bool CarXHonest;
    bool getXsends();

    //CarA

    void CarADetectionResult();
    bool CarAKnowsTruth;
    void storeTrustForXFromA();
    void selectInvolvedCars();
    void storeReputationForBFromA();
    bool isXMatchA();

    QPair<bool, double> descissionResult;

    //CarB
    QVector<double> reputations;
    void getReputatiosnBs();



};

#endif // INTERACTION_H
