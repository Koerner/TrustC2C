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

    returnPair.first = own.first * foreign.second; //multiply the percentages of trust
    returnPair.second = std::min(own.second,foreign.second); //return the smaller of the two list sizes.

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
        double sumPositive;
        double sumOverall;

        for(int i=0; i<trustList.size(); i++)
        {
            sumPositive += trustList.at(i).first * trustList.at(i).second;
            sumOverall += trustList.at(i).second;
        }
        returnPair.first = sumPositive/sumOverall;
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
        double sumPositive;
        double sumOverall;

        for(int i=0; i<reputationList.size(); i++)
        {
            sumPositive += reputationList.at(i).first * reputationList.at(i).second;
            sumOverall += reputationList.at(i).second;
        }
        returnPair.first = sumPositive/sumOverall;
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


