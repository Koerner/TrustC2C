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
    carXKnowsTruth = CarXDetectionResult();
    carXthinks = carXKnowsTruth == truth;
    qDebug() << "CarX thinks: " <<carXthinks;
    // Determine whether the car is honest.
    carXHonest = CarXHonestResult();
    carXsays = carXHonest == carXthinks;
    // Determine whether the carA identified the truth correctly.
    carAKnowsTruth = CarADetectionResult();
    carAthinks = carAKnowsTruth == truth;

    qDebug() << "CarX thinks: " <<carXsays << "CarA thinks: " << carAthinks << "is match: " << isXMatchA();

    //get all reputations from all Bs towards X as QList
    reputations = getReputatiosnBs();
    qDebug() << "Calculated reputations from B towards X, size: " << reputations.size();

    //calculate decission
    trustDecision tempDec;
    QList<QList<double>> reputationRecordABs;
    for(int i=2; i<carIDs.size(); i++)
    {
        reputationRecordABs.append(data->getCarReputation(carIDs.at(0), carIDs.at(i)));
    }
    descissionResult =  tempDec.calculateDecission(reputations, reputationRecordABs, data->getCarTrust(carIDs.at(0), carIDs.at(1)), carXsays);

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

QVector <long unsigned int> interaction::selectInvolvedCarsB2()
{
    /// determines the cars which are involved in level 2.
    /**   */

    return randCollection->carB2SelectRand.getCarID(instanceSettings.numTotalCars, instanceSettings.numCarsRecommending);

}

bool interaction::CarXDetectionResult()
{
    /// this gives back whether the carX knows the truth.
    /**   */

    return randCollection->detectRandX.getResultPercent(data->getCarPropDetects(carIDs.at(1)));
}


bool interaction::CarXHonestResult()
{
    /// this gives back whether the carX is honest and tells other cars the truth of it's knowledge.
    /**   */

    return randCollection->honestRand.getResultPercent(instanceSettings.PropHonestCarX);
}

bool interaction::CarADetectionResult()
{
    /// this gives back whether the carA knows the truth.
    /**   */
    return randCollection->detectRandA.getResultPercent(data->getCarPropDetects(carIDs.at(0)));

}


bool interaction::isXMatchA()
{
    return carXsays == carAthinks;
}

QList<double> interaction::getReputatiosnBs()
{
    /// This function gets all reputations from all Bs towards CarX and saves it in the <QList> reputations
    /**   */
    QList<double> reputationsReturn;

    trustKnowledge temp;

    for (int i=2; i < carIDs.size(); i++)
    {
        QList<double> temp = data->getCarReputation(carIDs.at(i), carIDs.at(1));
        if (temp.size() == 0 ) //hier noch tiefe abfrage
        {
            temp = generateHigherLevelReputation(1, carIDs.at(i), carIDs.at(1));
        }
        reputationsReturn.append(average::averageMean(temp));
    }

    return reputationsReturn;
}

QList<double> interaction::generateHigherLevelReputation(int depthRecomending, unsigned long int CarB, unsigned long CarX)
{
    qWarning() << "Hallo leer" ;

    QList<double> temp;

    int i = 1;
    while(temp.size()==0 && i < instanceSettings.numRecomendingDepth)
    {

    //QVector <long unsigned int> temp = interaction::selectInvolvedCarsB2();

    // wähle auto mit höchster id

    // frage ob es X kennen,
        //wenn jj summieren und speichern
        //wenn nein nächstes auto


     //wenn alle nein


    //frage bei diesen autos nach auto X

    //multipliziere reputation mit bekommenen werten

    }


    return temp;
}


void interaction::storeTrustForXFromA()
{
    /// This function handles the calculation of the new added trust value from A for X
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

    QList<double> newReputationValues = temp.reputationFeedback(isXMatchA(), reputations);

    for(int i=2; i < carIDs.size(); i++)
    {
        data->writeReputationB(carIDs.at(0), carIDs.at(i), newReputationValues.at(i-2));
    }
    qDebug() << "For cars" << carIDs << "the Reputation Feedback is: x x " << newReputationValues;

    //MATCH reputation with own experience.


}

void interaction::storeInteractionHandler()
{
    /// This stores the most important values of one event, which can be dispalyed in the gui later
    /**   */

    bool success = truth && carAthinks;
    data->writeInteractionLog(carIDs, truth, carXsays, success, descissionResult);
}
