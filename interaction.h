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

    database *data;
    settingsGUI instanceSettings;
    randStruct *randCollection;

    QVector<unsigned long int> carIDs;

    //Enviroment
    bool truth;
    void initTruth();
    void storeInteractionHandler();

    //CarX
    bool CarXDetectionResult();
    bool CarXHonestResult();

    bool carXKnowsTruth;
    bool carXthinks;
    bool carXHonest;
    bool carXsays;



    //CarA
    bool carAKnowsTruth;
    bool carAthinks;

    bool CarADetectionResult();


    //CarB
    QList<double> reputations;
    QList<double> getReputatiosnBs();
    QVector<unsigned long> selectInvolvedCarsB2();
    QList<double> generateHigherLevelReputation(int depthRecomending, unsigned long int CarB, unsigned long CarX);


    //calculation
    bool isXMatchA();
    void storeTrustForXFromA();
    void selectInvolvedCars();
    void storeReputationForBFromA();


    QPair<bool, double> descissionResult;





};

#endif // INTERACTION_H
