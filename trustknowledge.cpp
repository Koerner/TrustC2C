#include "trustknowledge.h"

trustKnowledge::trustKnowledge()
{

}



double trustKnowledge::getReputationFromBforX(QList<double> TrustValueListBX)
{
    /// obsolete right at the moment
    /**   */
    double reputation = average(TrustValueListBX);
    return reputation;
}

double trustKnowledge::trustFeedback(bool matching)
{
    /// Calculates a <double> for the event of a matching <bool> between carA and carX
    /**   */

    if(matching)
    {
        return(1.0);
    }
    else
    {
        return(-1.0);
    }
}

QVector<double> trustKnowledge::reputationFeedback(bool match, QVector<double> reputations)
{
    /// Calculates a <double> for the reputation of each recomending carB
    /**   */

    QVector<double> reputationsFeedbackValues;

    ///TODO sehr rudimentaer unten


    for(int i = 0; i<reputations.size(); i++)
    {
        if((reputations.at(i)>0.5 && match) || (reputations.at(i)<0.5 && !match) )
        {
            reputationsFeedbackValues.append(1);
        }
        else
        {
            reputationsFeedbackValues.append(-1);
        }
    }
    if(reputations.size() != reputationsFeedbackValues.size()){qFatal("reputation required and reputations in Feedback vector do not match");}

    return reputationsFeedbackValues;

}

double trustKnowledge::average(QList<double> elements)
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


