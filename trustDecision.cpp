#include "trustDecision.h"

trustDecision::trustDecision()
{

}

QPair<bool, double> trustDecision::calculateDecission(QList<QPair<double, int> > &reputationsBs, QPair<double, int> &trustAX, QPair<bool, double> &CarXsays, QPair<bool, double> &CarAthinks)
{
    /// This function returns a QPair, where the first is the descission about the reality and the secound the certainty this decsission is made.
    /**   */

    QPair<double, int> AXreputation = reputationWeightAverage(reputationsBs, trustAX); //3

    QPair<double, int> AXresult;
    QPair<bool, double> DecissionResult;

    AXresult.second = AXreputation.second;
    AXresult.first = AXreputation.first * CarXsays.second;

    if(CarAthinks.first == CarXsays.first)
    {
        DecissionResult.first = CarAthinks.first;
        DecissionResult.second = CarAthinks.second + AXresult.first;
        qDebug() << "A and X say the same";
    }
    else
    {
        if(CarAthinks.second > AXresult.first)
        {
            qDebug() << "Relied on my own value";
            DecissionResult.first = CarAthinks.first;
            DecissionResult.second = CarAthinks.second - AXresult.first;
        }
        else
        {
           qDebug() << "Relied on my own value";
           DecissionResult.first = CarXsays.first;
           DecissionResult.second = AXresult.first - CarAthinks.second;
        }
    }

    return DecissionResult;

}

QPair<double, int> trustDecision::reputationWeightAverage(QList<QPair<double, int>> reputationsBs, QPair<double, int> trustAX) //3
{
    QList<QPair<double, int>> allValues;
    allValues.append(trustAX);
    allValues.append(reputationsBs);
    QPair<double, int> returnPair;

    if(allValues.isEmpty())
    {
        returnPair.first = 0;
        returnPair.second = 0;
    }
    else
    {
        double weightedAverage = 0.0;
        int counter = 0;

        for(int i=0; i<allValues.size(); i++)
        {
            weightedAverage += allValues.at(i).first * allValues.at(i).second;
            counter += allValues.at(i).second;
        }

        QPair<double, int> returnPair;
        returnPair.first = weightedAverage / counter;
        returnPair.second = counter;
    }

    return returnPair;

}



int trustDecision::lastCheating(QList<double> elements)
{
    /// Returns the number until the last significant (>0,5) cheating attempt.
    /**   */

    int counter = 0;
    for(int i=0; i<elements.size(); i++)
    {
        if(elements.at(i) > -0.5)
        {
            counter++;
        }
        else
        {
            break;
        }
    }

    return counter;
}

int trustDecision::numberWrongRecomendations(QList<double> elements)
{
    /// Returns the number of wrong recomondations (<0).
    /**   */

    int counter = 0;
    for(int i=0; i<elements.size(); i++)
    {
        if(elements.at(i) < 0)
        {
            counter++;
        }
    }
    return counter;
}


