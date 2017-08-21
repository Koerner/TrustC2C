#include "interaction.h"


interaction::interaction(settingsGUI instanceSettingsHandover, randStruct &randCollectionHandover,  database& dataHandover)
{
    /// constructor. copies pointers of randGenerators so we can acces them later
    /**   */

    instanceSettings = instanceSettingsHandover;
    randCollection = &randCollectionHandover;

    data = &dataHandover;




    initTruth();


}


void interaction::initTruth()
{
    /// defines the truth. So if the situation (icy road is true or not)
    /**   */

    truth = randCollection->truthRand.getBool();
    qDebug() << "The reality is" << truth;

}





void interaction::run()
{
    /// this executes one interaction. Do not Run it twice, rather create a new class for doing so.
    /**   */
    // Determines Cars involved
    selectInvolvedCars();

    // Determine whether the carX identified the truth correctly.
    CarXDetectionResult();
    // Determine whether the car is honest.
    CarXHonestResult();
    // Determine whether the carA identified the truth correctly.
    CarADetectionResult();

    //get all reputations from all Bs towards X as QList
    getReputatiosnBs();

    //calculate decission
    trustDecision tempDec;
    QList<QList<double>> reputationRecordABs;
    for(int i=2; i<carIDs.size(); i++)
    {
        reputationRecordABs.append(data->getCarReputation(carIDs.at(0), carIDs.at(i)));
    }
    descissionResult =  tempDec.calculateDecission(reputations, reputationRecordABs, data->getCarTrust(carIDs.at(0), carIDs.at(1)), getXsends());

    // store new Trust from A towards X
    storeTrustForXFromA();

    //store Reputation from a towards B
    storeReputationForBFromA();

    //store interaction to database
    storeInteractionHandler();

}

void interaction::selectInvolvedCars()
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

    carIDs = randCollection->carSelectRand.getCarID(instanceSettings.numTotalCars, requiredCarIDs);
    qDebug() << carIDs;

}

void interaction::CarXDetectionResult()
{
    /// this gives back whether the carX knows the truth.
    /**   */
    CarXKnowsTruth = randCollection->detectRandX.getResultPercent(data->getCarPropDetects(carIDs.at(1)));
    qDebug() << "The probability for the carX knowing the truth is: " << instanceSettings.PropDetectsCarX <<" The result of random for knowing the truth is " << CarXKnowsTruth;
}



void interaction::CarXHonestResult()
{
    /// this gives back whether the carX is honest and tells other cars the truth of it's knowledge.
    /**   */
    CarXHonest = randCollection->honestRand.getResultPercent(instanceSettings.PropHonestCarX);
    qDebug() << "The probability for the car being honest is: " << instanceSettings.PropHonestCarX <<" The car is honest " << CarXHonest;
}

void interaction::CarADetectionResult()
{
    /// this gives back whether the carA knows the truth.
    /**   */
    CarAKnowsTruth = randCollection->detectRandA.getResultPercent(instanceSettings.PropDetectsCarA);
    qDebug() << "The probability for the carA knowing the truth is: " << instanceSettings.PropDetectsCarA <<" The result of random for knowing the truth is " << CarAKnowsTruth;
}

bool interaction::getXsends()
{
    bool xSend = ! (CarXKnowsTruth ^ CarXHonest);
    xSend = ! (xSend ^ truth);
    return xSend;
}

bool interaction::isXMatchA()
{


    //calculate car what A thinks is true afterwards
    bool aThinks = ! (truth ^ CarAKnowsTruth);
    qDebug() << "CarA thinks" << aThinks;

    bool match = ! (getXsends() ^ aThinks);

    return match;
}

void interaction::getReputatiosnBs()
{
    /// This function gets all reputations from all Bs towards CarX and saves it in the <QList> reputations
    /**   */
    reputations.clear();

    trustKnowledge temp;

    for (int i=2; i < carIDs.size(); i++)
    {
        QList<double> reputationBX;
        reputationBX = data->getCarReputation(carIDs.at(i), carIDs.at(1));
        reputations.append(temp.average(reputationBX));
    }
    qDebug() << "Calculated reputations from B towards X, size: " << reputations.size();
}


void interaction::storeTrustForXFromA()
{
    /// This function determines the trust value for X from A
    /**   */

    //calculate the Trust value which should be added
    trustKnowledge temp;
    double newTrustvalue = temp.trustFeedback(isXMatchA());

    //add the new trust value to the databse
    data->writeTrustX(carIDs.at(0), carIDs.at(1), newTrustvalue);
    qDebug() << "Added in knowlege of A: " << carIDs.at(0) << "a new Trust value for " << carIDs.at(1) << " : " << newTrustvalue;


}

void interaction::storeReputationForBFromA()
{
    /// This stores the change in reputation for carB from the perspective of carA
    /**   */


    trustKnowledge temp;
    if (reputations.size() != instanceSettings.numCarsRecommending){qFatal("no reputation specified, can't use it without calculation before");}

    QVector<double> newReputationValues = temp.reputationFeedback(isXMatchA(), reputations);

    for(int i=2; i < carIDs.size(); i++)
    {
        data->writeReputationB(carIDs.at(0), carIDs.at(i), newReputationValues.at(i-2));
    }
    qDebug() << "For cars" << carIDs << "the Reputation Feedback is: x x " << newReputationValues;

    //MATCH reputation with own experience.


}

void interaction::storeInteractionHandler()
{
    bool success = ! (truth ^ descissionResult.first);
    data->writeInteractionLog(carIDs, truth, getXsends(), success, descissionResult);
}
