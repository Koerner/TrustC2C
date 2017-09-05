#include "trustreputational.h"

trustReputational::trustReputational()
{

}

QPair<double, int> trustReputational::combine(QPair<double, int> own, QPair<double, int> foreign) //5
{
    /// Combines the the reputation Pairs.
    /**   */

    ///TODO: Here the size of both Pairs could be utilized better

    QPair<double, int> returnPair;

    returnPair.first = own.first * foreign.first; //multiply the percentages of trust
    returnPair.second = std::min(own.second,foreign.second); //return the smaller of the two list sizes.

    qDebug() <<"Result combine function: " << returnPair;
    return returnPair;
}



QPair<double, int> trustReputational::mergeTrust(QList<QPair<bool, double> > trustList) //6
{
    QPair<double, int> returnPair;


    returnPair.second = trustList.size();

    if(returnPair.second == 0)
    {
        returnPair.first = 0;
    }
    else{
        double sumPositive = 0;
        double sumOverall = 0;

        for(int i=0; i<trustList.size(); i++)
        {
            if(trustList.at(i).first)
            {
            sumPositive += trustList.at(i).second;
            }
            sumOverall += trustList.at(i).second;
        }
        if(sumOverall > 0.001)
        {
        returnPair.first = sumPositive/sumOverall;
        }
        else
        {
            returnPair.first = 0;
        }
    }

    return returnPair;
}

QPair<double, int> trustReputational::mergeReputation(QList<QPair<bool, double> > reputationList) //4
{
    QPair<double, int> returnPair;


    returnPair.second = reputationList.size();

    if(returnPair.second == 0)
    {
        returnPair.first = 0;
    }
    else{
        double sumPositive = 0;
        double sumOverall = 0;

        for(int i=0; i<reputationList.size(); i++)
        {
            if(reputationList.at(i).first)
            {
            sumPositive += reputationList.at(i).second;
            }
            sumOverall += reputationList.at(i).second;
        }
        if(sumOverall > 0.001)
        {
        returnPair.first = sumPositive/sumOverall;
        }
        else
        {
            returnPair.first = 0;
        }
    }

    return returnPair;
}

double trustReputational::mergeReputationD(QList<QPair<bool, double> > reputationList)
{
    QPair<double, int> temp = mergeReputation(reputationList);
    return temp.first;
}

double trustReputational::mergeTrustD(QList<QPair<bool, double> > trustList)
{
    QPair<double, int> temp = mergeTrust(trustList);
    return temp.first;
}


