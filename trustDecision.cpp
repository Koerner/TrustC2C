#include "trustDecision.h"

trustDecision::trustDecision()
{

}

QPair<bool, double> trustDecision::calculateDecission(QVector<double> reputationsBs, QList<QList<double>> reputationRecordABs, QList<double> trustRecordAX, double CarXsays)
{
    //calc over all reputation score from all Bs towards X including A's weight
    double reputationAX = 0.0;
    for(int i=0; i<reputationsBs.size(); i++)
    {

            reputationAX += reputationWeightCalc(reputationsBs.at(i), reputationAverageCalc(reputationRecordABs.at(i)));

    }

    //include A's own trust(!) opinion about X
    double trustAX = average(trustRecordAX);
    double overallTrustAX = trustWeightCalc(reputationAX, trustAX);

    //prepear return variable
    QPair<bool, double> tempReturn;
    tempReturn.first =  CarXsays;
    tempReturn.second =  overallTrustAX;

    return tempReturn;



}

double trustDecision::reputationWeightCalc(double reputationBX, double reputationAB)
{
    double overallReputationABX;

    ///TODO etwas primitive hier :)

    overallReputationABX = reputationBX * reputationAB;

    return overallReputationABX;
}

double trustDecision::trustWeightCalc(double reputationBX, double trustAX)
{
    double overallTrustAX = 0;
    overallTrustAX = (reputationBX + trustAX * 3) / 4;
    return overallTrustAX;
}

double trustDecision::reputationAverageCalc(QList<double> elements)
{
    ///TODO auch primitiv :)
    return average(elements);
}

double trustDecision::average(QList<double> elements)
{
    {
        if(elements.size() == 0)
        {
            return 0;
        }
        else
        {
            double sum = 0;
            for(int i=0; i<elements.size(); i++)
            {
                sum = elements.at(i);
            }
            return sum / elements.size();
        }
    }
}
