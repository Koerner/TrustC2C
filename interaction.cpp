#include "interaction.h"


interaction::interaction(QVector<int> &settingsVector, randomQ& truthRandHandover, randomQ& detectRandHandover, randomQ& honestRandHandover)
{
    /// constructor. copies pointers of randGenerators so we can acces them later
    /**   */

    truthRand = &truthRandHandover;
    detectRand = &detectRandHandover;
    honestRand = &honestRandHandover;

    //copy settings from settingsVector
    probCarDetects = settingsVector.at(1); //Probability that the car knows what is true
    probCarHonest = settingsVector.at(2); //Probability that the car tells what it thinks is true

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

    // Determine whether the car identified the truth correctly.
    CarDetectionResult();
    CarHonestResult();



}

void interaction::CarDetectionResult()
{
    CarKnowsTruth = detectRand->getResultPercent(probCarDetects);
    qDebug() << "The probability for the car knowing the truth is: " << probCarDetects <<" The result of random for knowing the truth is " << CarKnowsTruth;
}

void interaction::CarHonestResult()
{
    CarHonest = honestRand->getResultPercent(probCarHonest);
    qDebug() << "The probability for the car being honest is: " << probCarHonest <<" The car is honest " << CarHonest;
}

