#include "interaction.h"


interaction::interaction(settingsGUI instanceSettingsHandover, randStruct &randCollectionHandover,  database& dataHandover, logDatabase& logdataHandover)
{
    /// constructor. copies necessary stuff.
    /** Copies pointers of randGenerators, pointer to the database and the Settings from the GUI  */

    instanceSettings = instanceSettingsHandover;
    randCollection = &randCollectionHandover;
    data = &dataHandover;
    logdata = &logdataHandover;

}


void interaction::run()
{
    /// this function executes one interaction. Do not run it twice, rather create a new class for doing so.
    /**   */

    qDebug() << "################## New interaction ###################";

    // Defines whether the reality is true or false.
    truth = randCollection->truthRand.getBool();

    // Determines Cars involved
    carIDs = selectInvolvedCars();
    qDebug() << "The list of involved cars is: " << carIDs;

    // Determine whether the carX identified the truth correctly.
    carXKnowsTruth = getCarXKnowsTruth();
    carXthinks = carXKnowsTruth.first == truth;
    // Determine whether the car is honest.
    carXHonest = isCarXHonest();
    carXsays = getCarXsays();

    // Determine whether the carA identified the truth correctly.

    carAobservation = getCarAobservation();
    carAprediction = getCarAprediction();

    qDebug() << "CarX says: " <<carXsays << "| CarA prediction is " << carAprediction << "observation is:" << carAobservation <<"| is match: " << isXMatchAobservation();

    //get all reputations from all Bs towards X as QList
    reputations = getReputatiosnBs();
    qDebug() << "Calculated reputations from B towards X: " << reputations << ", size: " << reputations.size();

    //megre foreign reputation values with own reputatuin towards B
    reputationABXs =  getMergedReputatiosnABXs(reputations);
    qDebug() << "Calculated reputations from ABX combined: " << reputationABXs << ", size: " << reputationABXs.size();


    //calculate decission
    descissionResult = getDecissionA();
    correctDecission = truth == descissionResult.first;

    // store new Trust from A towards X
    storeTrustForXFromA();

    //store Reputation from a towards B
    storeReputationForBFromA();

    //store interaction to database
    logInteraction();

}

QVector<unsigned long int> interaction::selectInvolvedCars()
{
    /// determines the cars involved in the interaction.
    /**   */
    int requiredCarIDs = 1 + 1 + instanceSettings.numCarsRecommending;

    if (requiredCarIDs > data->sizeCarsVector())
    {
        qWarning() << "Not enough total cars for the number of reomending cars. Setting the reccomending cars to #totalCars-2.";
        instanceSettings.numCarsRecommending = data->sizeCarsVector() - 2;
        requiredCarIDs = data->sizeCarsVector();
    }

    QVector<unsigned long int> cars;
    cars.clear();
    cars.append(randCollection->carSelectRandAX.getCarID(data->sizeCarsVector(),2, cars));
    cars.append(randCollection->carSelectRandB.getCarID(data->sizeCarsVector(), instanceSettings.numCarsRecommending, cars));
    return cars;

}



QPair<bool, double> interaction::getCarXKnowsTruth()
{
    /// this gives back whether the carX knows the truth.
    /**   */

    //qDebug() << "CarX %:" << data->getCarPropDetectsPrediction(carIDs.at(0));
    double percent = data->getCarPropDetectsX(carIDs.at(1));
    double stddistribution = data->getCarPropDetectsXStdDist(carIDs.at(1));
    percent = randCollection->Poison.getPoisonPercent(percent, stddistribution);
    qDebug() << "%:" << percent;

    QPair<bool, double> returnPair;
    returnPair.first = randCollection->detectRandX.getResultPercent(percent);
    returnPair.second = percent;

    return returnPair;
}


bool interaction::isCarXHonest()
{
    /// this gives back whether the carX is honest and tells other cars the truth of it's knowledge.
    /**   */
    double percent = data->getCarPropHonest(carIDs.at(1));
    double stddistribution = data->getCarPropHonestStdDist(carIDs.at(1));
    percent = randCollection->Poison.getPoisonPercent(percent, stddistribution);

    return randCollection->honestRand.getResultPercent(percent);
}

QPair<bool, double> interaction::getCarXsays()
{
    QPair<bool, double> returnPair;

    returnPair.first = carXHonest == carXthinks;
    if(instanceSettings.certaintyXon)
    {
        returnPair.second = carXKnowsTruth.second; //if certainty is on
    }
    else
    {
        returnPair.second = 1; //no certainty added, tus 100%
    }

    return returnPair;
}

QPair<bool, double> interaction::getCarAobservation()
{
    QPair<bool, double> returnPair;
    double percent = data->getCarPropDetectsObservation(carIDs.at(0));
    double stddistribution = data->getCarPropDetectsObservationStdDist(carIDs.at(0));
    percent = randCollection->Poison.getPoisonPercent(percent, stddistribution);

    bool carAKnowsTruth = randCollection->detectRandA.getResultPercent(percent);

    returnPair.first = carAKnowsTruth == truth;
    returnPair.second = percent;

    return returnPair;
}

QPair<bool, double> interaction::getCarAprediction()
{
    QPair<bool, double> returnPair;
    double percent = data->getCarPropDetectsPrediction(carIDs.at(0));
    double stdDistribution = data->getCarPropDetectsPredictionStdDist(carIDs.at(0));
    percent = randCollection->Poison.getPoisonPercent(percent, stdDistribution);


    bool carAKnowsTruth = randCollection->detectRandA.getResultPercent(percent);

    returnPair.first = carAKnowsTruth == truth;
    returnPair.second = percent;

    return returnPair;
}




bool interaction::isXMatchAobservation()
{
    return carXsays.first == carAobservation.first;
}

///TODO move the following three functions to trustreputational

QList<QPair<double, int>> interaction::getReputatiosnBs()
{
    /// This function returns a QList<double> of all Bs involved in the scene.
    /// for each B get all trust values from B towards X, compute average and save to QList
    /**   */
    QList<QPair<double, int>> reputationsReturn;
    trustReputational tempTR;

    for (int i=2; i < carIDs.size(); i++)
    {
        if(REP_DEBUG){qDebug() << "--- Start for Car " << carIDs.at(i) << " ---";};

        QList<QPair<bool, double>> wholeTrustRecordBX = data->getCarTrust(carIDs.at(i), carIDs.at(1));
        QPair<double, int> mergedTrustBX;

        if(wholeTrustRecordBX.isEmpty()  && instanceSettings.maxRecomendingDepth > 1)
        {
            // BB Level
            if(REP_DEBUG){qDebug() << "Car " << carIDs.at(i) << " needs new level";};
            mergedTrustBX = getHigherLevelReputation((instanceSettings.maxRecomendingDepth - 1),  carIDs.at(i), carIDs);
        }
        else
        {
            // B Level

            mergedTrustBX = tempTR.mergeTrust(wholeTrustRecordBX);
            if(REP_DEBUG){qDebug() << "Car " << carIDs.at(i) << " has at least one trust value abot CarX. Result: "<< mergedTrustBX;};
        }

        reputationsReturn.append(mergedTrustBX);

        if(REP_DEBUG){qDebug() << "--- End for Car " << carIDs.at(i) << "Reputation towards CarX:" << reputationsReturn.last() <<" ---";};
    }

    return reputationsReturn;
}

QList<QPair<double, int>> interaction::getMergedReputatiosnABXs(QList<QPair<double, int>> reputationBs)
{
    QList<QPair<double, int>> returnMergedReputatiosnABX;

    for (int i=2; i < reputationBs.size()+2; i++)
    {
        QPair<double, int> mergedreputationsAB;
        QList<QPair<bool, double>> reputationList = data->getCarReputation(carIDs.at(0), carIDs.at(i));

        if (reputationList.isEmpty()) //if no reputation is available set it 50%
        {
            QPair<bool, double> temp = qMakePair(true, 0.1);
            reputationList.append(temp);
            temp = qMakePair(false, 0.1);
            reputationList.append(temp);
        }


        mergedreputationsAB = trustReputational::mergeReputation(reputationList);

        if(REP_DEBUG){qDebug() << "Reputation A towards B :"<< reputationList <<"=>" << mergedreputationsAB <<" ---";};
        if(REP_DEBUG){qDebug() << "Merged A->B :" << mergedreputationsAB;};

        returnMergedReputatiosnABX.append(trustReputational::combine(mergedreputationsAB, reputationBs.at(i-2)));
    }

    return returnMergedReputatiosnABX;
}

QPair<double, int> interaction::getHigherLevelReputation(int depthRecomending, unsigned long carB, QVector<unsigned long>blockedCarIDs)
{
    if(REP_DEBUG){qDebug() << "+++ Start level " << instanceSettings.maxRecomendingDepth - depthRecomending  <<"to car" << carB << "'s recomendations.";};

    QPair<int, double> recomendationReturn;
    trustReputational tempTR;

    QList<unsigned long> tempCarsAscending = data->getCarIDsAscendingReputation(carB);
    int i = tempCarsAscending.size() -1;

    for (i=i; i>0 && i>(tempCarsAscending.size() -1 -instanceSettings.maxLevelWidth); i--) //going down the ascending list
    {
        if(REP_DEBUG){qDebug() << "~~~ Start for Car " << carB << "<-" <<tempCarsAscending.at(i)<<"~~~";};
        QVector<unsigned long> newBlockedCarIDs = blockedCarIDs;
        if(blockedCarIDs.contains(tempCarsAscending.at(i))) //making sure there is no loop
        {
            //loop detected
        }
        else
        {
            newBlockedCarIDs.append(tempCarsAscending.at(i));
            QList<QPair<bool, double>> wholeTrustRecordBX = data->getCarTrust(tempCarsAscending.at(i), carIDs.at(1));
            QPair<double, int> mergedreputationsBX;
            if(wholeTrustRecordBX.isEmpty()  && depthRecomending > 1)
            {
                //BBB Level
                mergedreputationsBX = (getHigherLevelReputation((depthRecomending - 1), tempCarsAscending.at(i), newBlockedCarIDs));
            }
            else
            {
                mergedreputationsBX = tempTR.mergeTrust(wholeTrustRecordBX);
            }

            QPair<double, int> mergedreputationsAB;
            mergedreputationsAB = tempTR.mergeReputation(data->getCarReputation(carB, tempCarsAscending.at(i)));

            recomendationReturn = (tempTR.combine(mergedreputationsAB, mergedreputationsBX));

            if(recomendationReturn.second != 0)
            {
                i=0;
            }
        }
        if(REP_DEBUG){qDebug() << "~~~ Car " << carB << "<-" <<tempCarsAscending.at(i)<<"<--" << recomendationReturn.first;};
        if(REP_DEBUG){qDebug() << "~~~ End for Car " << carB << "<-" <<tempCarsAscending.at(i)<<"~~~";};
    }

    if(REP_DEBUG){qDebug() << "+++ End level " << instanceSettings.maxRecomendingDepth - depthRecomending << " for car " << carB;};

    return recomendationReturn;

}



void interaction::storeTrustForXFromA()
{
    /// This function handles the calculation of the new added trust value from A for X
    /**   */

    //calculate the Trust value which should be added
    trustKnowledge temp;
    QPair<bool, double> newTrustvalue = trustKnowledge::trustFeedback(isXMatchAobservation(), carXsays.second);

    //add the new trust value to the databse
    data->writeTrustX(carIDs.at(0), carIDs.at(1), newTrustvalue);
    qDebug() << "Added ne trust value : " << carIDs.at(0) << "->" << carIDs.at(1) << " : " << newTrustvalue;


}

void interaction::storeReputationForBFromA()
{
    /// This stores the change in reputation for carB from the perspective of carA
    /**   */


    trustKnowledge temp;
    //if (reputations.size() != instanceSettings.numCarsRecommending){qFatal("no reputation specified, can't use it without calculation before");}

    QList<QPair<bool,double>> newReputationValues = temp.reputationFeedback(isXMatchAobservation(), carAobservation.second, reputations);

    for(int i=2; i < carIDs.size(); i++)
    {
        data->writeReputationB(carIDs.at(0), carIDs.at(i), newReputationValues.at(i-2));
    }
    qDebug() << "For cars" << carIDs << "the Reputation Feedback is: x x " << newReputationValues;

    //MATCH reputation with own experience.


}

QPair<bool, double> interaction::getDecissionA()
{
    QPair<bool, double> returnPair;
    QPair<double, int> trustAX;
    trustAX = trustReputational::mergeTrust(data->getCarTrust(carIDs.at(0), carIDs.at(1)));

    trustDecision temp;
    returnPair = temp.calculateDecission(reputationABXs, trustAX, carXsays, carAprediction);


    return returnPair;
}

void interaction::logInteraction()
{
    /// This stores the most important values of one event, which can be dispalyed in the gui later
    /**   */
    interactionLog log;
    //log.carAKnowsTruth = carAKnowsTruth;
    log.carAprediction = carAprediction;
    log.carAobservation = carAobservation;
    log.carIDs = carIDs;
    log.carXHonest = carXHonest;
    log.carXKnowsTruth = carXKnowsTruth;
    log.carXsays = carXsays;
    log.carXthinks = carXthinks;
    log.correctDecission = correctDecission;
    log.descissionResult = descissionResult;
    log.reputations = reputations;
    log.reputationABXs = reputationABXs;
    log.truth = truth;

    logdata->writeInteractionLog(log);
}
