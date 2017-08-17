#include "interaction.h"


interaction::interaction(QVector<int> &settingsVector, randomQ& truthRandHandover, randomQ& detectRandXHandover, randomQ& honestRandHandover, randomQ& detectRandAHandover)
{
    /// constructor. copies pointers of randGenerators so we can acces them later
    /**   */

    truthRand = &truthRandHandover;
    detectRandX = &detectRandXHandover;
    honestRand = &honestRandHandover;
    detectRandA = &detectRandAHandover;

    //copy settings from settingsVector for CarX
    probCarXDetects = settingsVector.at(1); //Probability that the carX knows what is true
    probCarXHonest = settingsVector.at(2); //Probability that the car tells what it thinks is true

    //copy settings from settingsVector for CarA
    probCarADetects = settingsVector.at(3); //Probability that the carA knows what is true


    initTruth();


}


void interaction::initTruth()
{
    /// defines the truth. So if the situation (icy road is true or not)
    /**   */

    truth = truthRand->getBool();
    qDebug() << "The reality is" << truth;

}



void interaction::run()
{
    /// this executes one interaction. Do not Run it twice, rather create a new class for doing so.
    /**   */

    // Determine whether the carX identified the truth correctly.
    CarXDetectionResult();
    // Determine whether the car is honest.
    CarXHonestResult();
    // Determine whether the carA identified the truth correctly.
    CarADetectionResult();

    determineTrustForXFromA();

}



void interaction::CarXDetectionResult()
{
    /// this gives back whether the carX knows the truth.
    /**   */
    CarXKnowsTruth = detectRandX->getResultPercent(probCarXDetects);
    qDebug() << "The probability for the carX knowing the truth is: " << probCarXDetects <<" The result of random for knowing the truth is " << CarXKnowsTruth;
}



void interaction::CarXHonestResult()
{
    /// this gives back whether the carX is honest and tells other cars the truth of it's knowledge.
    /**   */
    CarXHonest = honestRand->getResultPercent(probCarXHonest);
    qDebug() << "The probability for the car being honest is: " << probCarXHonest <<" The car is honest " << CarXHonest;
}

void interaction::CarADetectionResult()
{
    /// this gives back whether the carA knows the truth.
    /**   */
    CarAKnowsTruth = detectRandA->getResultPercent(probCarADetects);
    qDebug() << "The probability for the carA knowing the truth is: " << probCarADetects <<" The result of random for knowing the truth is " << CarAKnowsTruth;
}

void interaction::determineTrustForXFromA()
{
    /// This function determines the trust value for X from A
    /**   */

    //calculate what X sends
    bool xSend = ! (CarXKnowsTruth ^ CarXHonest);
    xSend = ! (xSend ^ truth);
    qDebug() << "CarX sends" << xSend;

    //calculate car what A thinks is true afterwards
    bool aThinks = ! (truth ^ CarAKnowsTruth);
    qDebug() << "CarA thinks" << aThinks;



    if(xSend == aThinks)
    {
        qDebug() << "Increasing Trust";
    }
    else
    {
        qDebug() << "Decreasing Trust";
    }
}
