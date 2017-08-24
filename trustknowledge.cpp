#include "trustknowledge.h"

trustKnowledge::trustKnowledge()
{

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

QList<double> trustKnowledge::reputationFeedback(bool match, QList<double> &reputations)
{
    /// Calculates a <double> for the reputation of each recomending carB
    /**   */

    QList<double> reputationsFeedbackValues;

    for(int i = 0; i<reputations.size(); i++)
    {
        if((reputations.at(i)>0 && match) || (reputations.at(i)<0 && !match) )
        {
            reputationsFeedbackValues.append(reputations.at(i) * 1);
        }
        else if((reputations.at(i)<0 && match) || (reputations.at(i)>0 && !match) )
        {
            reputationsFeedbackValues.append(-1 * reputations.at(i));
        }
        else
        {
            reputationsFeedbackValues.append(0);
        }
    }
    if(reputations.size() != reputationsFeedbackValues.size()){qFatal("reputation required and reputations in Feedback vector do not match");}

    return reputationsFeedbackValues;

}




