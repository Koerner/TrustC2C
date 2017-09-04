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
#include "logdatabase.h"
#include "trustreputational.h"


class interaction
{
public:
    interaction(settingsGUI instanceSettingsHandover, randStruct &randCollectionHandover, database &dataHandover, logDatabase &logdataHandover);

    void run();

private:

    database *data;
    logDatabase *logdata;
    settingsGUI instanceSettings;
    randStruct *randCollection;

    //Enviroment
    bool truth;
    QVector<unsigned long int> carIDs;
    QVector<unsigned long> selectInvolvedCars();

    //CarX
    bool getCarXKnowsTruth();
    bool isCarXHonest();
    QPair<bool,double> getCarXsays();

    bool carXKnowsTruth;
    bool carXthinks;
    bool carXHonest;
    QPair<bool,double> carXsays;


    //CarA
    bool carAKnowsTruth;
    QPair<bool,double> carAthinks;
    QPair<bool,double> getCarAthinks();

    bool getCarAKnowsTruth();


    //CarB
    QList<QPair<double,int>> reputations;
    QList<QPair<double,int>> getReputatiosnBs();
    QPair<double, int> getHigherLevelReputation(int depthRecomending, int neededRecomendingWidth, unsigned long carB, QVector<unsigned long> blockedCarIDs);



    //calculation
    bool isXMatchA();

    //TrustRecord
    void storeTrustForXFromA();
    void storeReputationForBFromA();

    //Decission
    QPair<bool, double> descissionResult;
    QPair<bool, double> getDecissionA();
    bool correctDecission;

    //Logging
    void logInteraction();

};

#endif // INTERACTION_H
