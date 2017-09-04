#include "trustknowledge.h"

trustKnowledge::trustKnowledge()
{

}

QPair<bool, double> trustKnowledge::trustFeedback(bool matching, double sentCertainty)
{
    /// Calculates a <double> for the event of a matching <bool> between carA and carX
    /**   */
    QPair<bool, double> returnPair;
    returnPair.first = matching;
    returnPair.second = sentCertainty;

    return returnPair;
}

QList<QPair<bool, double>> trustKnowledge::reputationFeedback(bool match, QList<QPair<double,int>> &reputations)
{
    /// Calculates a <double> for the reputation of each recomending carB
    /**   */

    QList<QPair<bool, double>> reputationsFeedbackValues;

    for(int i = 0; i<reputations.size(); i++)
    {
        QPair<bool, double> temp;
        temp.first = match;
        temp.second = reputations.at(i).first;
        reputationsFeedbackValues.append(temp);
    }


    return reputationsFeedbackValues;

}




