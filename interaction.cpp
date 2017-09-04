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
    carXthinks = carXKnowsTruth == truth;
    // Determine whether the car is honest.
    carXHonest = isCarXHonest();
    carXsays = getCarXsays() ;
    // Determine whether the carA identified the truth correctly.
    carAKnowsTruth = getCarAKnowsTruth();
    carAthinks = getCarAthinks();carAKnowsTruth == truth;

    qDebug() << "CarX thinks: " <<carXsays << "| CarA thinks: " << carAthinks << "| is match: " << isXMatchA();

    //get all reputations from all Bs towards X as QList
    reputations = getReputatiosnBs();
    qDebug() << "Calculated reputations from B towards X: " << reputations << ", size: " << reputations.size();

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

    if (requiredCarIDs > instanceSettings.numTotalCars)
    {
        qWarning() << "Not enough total cars for the number of reomending cars. Setting the reccomending cars to #totalCars-2.";
        instanceSettings.numCarsRecommending = instanceSettings.numTotalCars - 2;
        requiredCarIDs = instanceSettings.numTotalCars;
    }

    return randCollection->carSelectRand.getCarID(instanceSettings.numTotalCars, requiredCarIDs);

}



bool interaction::getCarXKnowsTruth()
{
    /// this gives back whether the carX knows the truth.
    /**   */

    return randCollection->detectRandX.getResultPercent(data->getCarPropDetects(carIDs.at(1)));
}


bool interaction::isCarXHonest()
{
    /// this gives back whether the carX is honest and tells other cars the truth of it's knowledge.
    /**   */

    return randCollection->honestRand.getResultPercent(instanceSettings.PropHonestCarX);
}

QPair<bool, double> interaction::getCarXsays()
{
    QPair<bool, double> returnPair;

    returnPair.first = carXHonest == carXthinks;
    returnPair.second = 1; ///TODO if we want to add certainty

    return returnPair;
}

QPair<bool, double> interaction::getCarAthinks()
{
    QPair<bool, double> returnPair;
    returnPair.first = carAKnowsTruth == truth;
    returnPair.second = data->getCarPropDetects(carIDs.at(0));

    return returnPair;
}

bool interaction::getCarAKnowsTruth()
{
    /// this gives back whether the carA knows the truth.
    /**   */
    return randCollection->detectRandA.getResultPercent(data->getCarPropDetects(carIDs.at(0)));

}


bool interaction::isXMatchA()
{
    return carXsays.first == carAthinks.first;
}

QList<QPair<double, int>> interaction::getReputatiosnBs()
{
    /// This function returns a QList<double> of all Bs involved in the scene.
    /// for each B get all trust values from B towards X, compute average and save to QList
    /**   */
    QList<QPair<double, int>> reputationsReturn;

    for (int i=2; i < carIDs.size(); i++)
    {
        if(REP_DEBUG){qDebug() << "--- Start for Car " << carIDs.at(i) << " ---";};

        QList<QPair<bool, double>> wholeTrustRecordBX = data->getCarTrust(carIDs.at(i), carIDs.at(1));
        QPair<double, int> mergedTrustBX;

        if(wholeTrustRecordBX.isEmpty()  && instanceSettings.maxRecomendingDepth > 1)
        {
            // BB Level
            mergedTrustBX = getHigherLevelReputation((instanceSettings.maxRecomendingDepth - 1), instanceSettings.minRecomendingWidth, carIDs.at(i), carIDs);
        }
        else
        {
            // B Level
            mergedTrustBX = trustReputational::mergeTrust(wholeTrustRecordBX);
        }


        QPair<double, int> mergedreputationsAB;
        mergedreputationsAB = trustReputational::mergeReputation(data->getCarReputation(carIDs.at(0), carIDs.at(i)));

        reputationsReturn.append(trustReputational::combine(mergedreputationsAB, mergedTrustBX));

        if(REP_DEBUG){qDebug() << "--- End for Car " << carIDs.at(i) << " ---";};
    }

    return reputationsReturn;
}

QPair<double, int> interaction::getHigherLevelReputation(int depthRecomending, int neededRecomendingWidth, unsigned long carB, QVector<unsigned long>blockedCarIDs)
{
    if(REP_DEBUG){qDebug() << "+++ Start level " << instanceSettings.maxRecomendingDepth - depthRecomending << " to add " << neededRecomendingWidth <<"to car" << carB << "'s recomendations.";};

    QPair<int, double> recomendationReturn;

    QList<unsigned long> tempCarsAscending = data->getCarIDsAscendingReputation(carB);
    int i = tempCarsAscending.size() -1;

    for (i; i>0; i--) //going down the ascending list
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
                mergedreputationsBX = (getHigherLevelReputation((depthRecomending - 1), (neededRecomendingWidth), tempCarsAscending.at(i), newBlockedCarIDs));
            }
            else
            {
                mergedreputationsBX = trustReputational::mergeTrust(wholeTrustRecordBX);
            }
                QPair<double, int> mergedreputationsAB;
                mergedreputationsAB = trustReputational::mergeReputation(data->getCarReputation(carB, tempCarsAscending.at(i)));

                recomendationReturn = (trustReputational::combine(mergedreputationsAB, mergedreputationsBX));
            if(recomendationReturn.second != 0)
            {
                i=0;
            }
        }
        if(REP_DEBUG){qDebug() << "~~~ End for Car " << carB << "<-" <<tempCarsAscending.at(i)<<"~~~";};
    }

    if(REP_DEBUG){qDebug() << "+++ End level " << instanceSettings.maxRecomendingDepth - depthRecomending << " for car " << carB;};

    return recomendationReturn;

}

//QList<QPair<double,int>> interaction::generateHigherLevelReputation(int depthRecomending, int neededRecomendingWidth, unsigned long carB)
//{
//    qDebug() << "Started level " << instanceSettings.maxRecomendingDepth - depthRecomending << "to get the number of  "<< neededRecomendingWidth << "required values.";

//    QList<QPair<double,int>> tempReturn;


//    QList<unsigned long> tempCarsAscending = data->getCarIDsAscendingReputation(carB);
//    int j = tempCarsAscending.size() -1;

//    while(tempReturn.size() < neededRecomendingWidth && j >= 0)
//    {
//        qDebug() << "+++ Car: " << tempCarsAscending.at(j);
//        QList<double> tempList = data->getCarTrust(carIDs.at(tempCarsAscending.at(j)),carIDs.at(1));
//        if(!tempList.isEmpty())
//        {
//            QList<double> factorList = data->getCarReputation(carB, tempCarsAscending.at(j));
//            double factor = average::averageMean(factorList).first;
//            <QPair <double, int> temp = verage::averageMean(tempList);
//            temp.second = temp.second * factor;
//            tempReturn.append(temp);
//        }
//        j--;
//    }

//     qDebug() << "Got "<< tempReturn.size() << "recomendations";


//    if((tempReturn.size() < neededRecomendingWidth) && (depthRecomending > 1))
//    {
//        qDebug() << "At this level there are still not enough recommendations. " << neededRecomendingWidth - tempReturn.size() << " more needed";

//        int k = tempCarsAscending.size() - 1;
//        qDebug() << "Will start the next level with the following cars: " << tempCarsAscending << " Total number of cars: " << k+1;

//        while((neededRecomendingWidth - tempReturn.size() > 0) && (k >= 0))
//        {
//            QList<double> temp2 = generateHigherLevelReputation((depthRecomending-1), (instanceSettings.minRecomendingWidth - tempReturn.size()), tempCarsAscending.at(k));
//            if(!temp2.isEmpty())
//            {
//                //QList<double> factorList = data->getCarReputation(carB, tempCarsAscending.at(k));
//                //double factor = average::averageMean(factorList);
//                tempReturn.append(average::averageMean(temp2));
//            }
//            k--;
//        }

//    }
//    return tempReturn;
//}

//QList<double> interaction::OLDgenerateHigherLevelReputation(int depthRecomending, int neededRecomendingWidth, QList<unsigned long> cars)
//{
//    qWarning() << "Started level " << instanceSettings.maxRecomendingDepth - depthRecomending << "to get the number of  "<< neededRecomendingWidth << "required values.";

//    QList<double> tempReturn;

//    //return highest remonding car of B
//    int i=0;
//    while( (tempReturn.size() < neededRecomendingWidth) && (i < cars.size()))
//    {


//        QList<unsigned long> tempAscending = data->getCarIDsAscendingReputation(cars.at(i));

//        int j = tempAscending.size() -1;
//        QList<double> tempReturn2;
//        while(tempReturn2.size() < neededRecomendingWidth && j >= 0)
//        {
//            QList<double> temp = data->getCarTrust(cars.at(tempAscending.at(j)),cars.at(1));
//            if(!temp.isEmpty())
//            {
//                tempReturn2.append(average::averageMean(temp));
//            }
//            j--;
//        }
//        if(!tempReturn2.isEmpty())
//        {
//            tempReturn.append(average::averageMean(tempReturn2));
//        }
//        i++;
//    }


//    if((tempReturn.size() < neededRecomendingWidth) && (depthRecomending > 1))
//    {
//        qDebug() << "At this level there are still not enough recommendations. " << neededRecomendingWidth - tempReturn.size() << " more needed";

//        QList<unsigned long>carsNew;

//        for (int k=0; k < cars.size(); k++)
//        {
//        carsNew.append(data->getCarIDsAscendingReputation(cars.at(k)));
//        }

//        qDebug() << "Will start the next level with the following cars: " << carsNew << " Total number of cars: " << carsNew.size();

//        if(!carsNew.isEmpty())
//        {
//            QList<double> temp2 = generateHigherLevelReputation((depthRecomending-1), (instanceSettings.minRecomendingWidth - tempReturn.size()), carsNew);
//            if(temp2.size() != 0)
//            {
//                tempReturn.append(average::averageMean(temp2));
//            }
//        }

//    }
//    return tempReturn;
//}


void interaction::storeTrustForXFromA()
{
    /// This function handles the calculation of the new added trust value from A for X
    /**   */

    //calculate the Trust value which should be added
    trustKnowledge temp;
    QPair<bool, double> newTrustvalue = trustKnowledge::trustFeedback(isXMatchA(), carXsays.second);

    //add the new trust value to the databse
    data->writeTrustX(carIDs.at(0), carIDs.at(1), newTrustvalue);
    qDebug() << "Added ne trust value : " << carIDs.at(0) << "->" << carIDs.at(1) << " : " << newTrustvalue;


}

void interaction::storeReputationForBFromA()
{
    /// This stores the change in reputation for carB from the perspective of carA
    /**   */


    trustKnowledge temp;
    if (reputations.size() != instanceSettings.numCarsRecommending){qFatal("no reputation specified, can't use it without calculation before");}

    QList<QPair<bool,double>> newReputationValues = temp.reputationFeedback(isXMatchA(), reputations);

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
    QList<QList<QPair<bool,double>>> reputationRecordABs;

    for(int i=2; i<carIDs.size(); i++)
    {
        // gets all reputations (!) from A to one B
        reputationRecordABs.append(data->getCarReputation(carIDs.at(0), carIDs.at(i)));
    }
    QPair<double, int> trustAX;
    trustAX = trustReputational::mergeTrust(data->getCarTrust(carIDs.at(0), carIDs.at(1)));

    trustDecision temp;
    returnPair = temp.calculateDecission(reputations, trustAX, carXsays, carAthinks);


    return returnPair;
}

void interaction::logInteraction()
{
    /// This stores the most important values of one event, which can be dispalyed in the gui later
    /**   */
    interactionLog log;
    log.carAKnowsTruth =carAKnowsTruth;
    log.carAthinks = carAthinks;
    log.carIDs = carIDs;
    log.carXHonest = carXHonest;
    log.carXKnowsTruth = carXKnowsTruth;
    log.carXsays = carXsays;
    log.carXthinks = carXthinks;
    log.correctDecission = correctDecission;
    log.descissionResult = descissionResult;
    log.reputations = reputations;
    log.truth = truth;

    logdata->writeInteractionLog(log);
}
