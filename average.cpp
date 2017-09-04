#include "average.h"

average::average()
{

}

QPair<double, int> average::averageMean(QList<double> &elements)
{
    qErrnoWarning("Soll die average funktion hier wirklich verwendet werden?");
    QPair<double, int> returnPair;

    if(elements.size() == 0)
    {
        returnPair.first = 0;
        returnPair.second = 0;
        return returnPair;
    }
    else if(elements.size() == 1)
    {
        returnPair.first = elements.at(0);
        returnPair.second = 1;
        return returnPair;
    }
    else
    {
        double sum = 0;
        int counter = 0;

        for(int i=0; i<elements.size(); i++)
        {
            if(elements.at(i) != 0)
            {
                sum += elements.at(i);
                counter++;
            }
        }

        if(counter == 0)
        {
            returnPair.first = 0;
            returnPair.second = 0;
            return returnPair;

        }
        else
        {
            returnPair.first = sum /  counter;
            returnPair.second = counter;
            return returnPair;
        }

    }
}

double average::sum(QList<double> &elements)
{
    double sum = 0;

    for(int i=0; i<elements.size(); i++)
    {
        sum += elements.at(i);
    }
    return sum;

}

double average::weightedSum(QList<double> &weight, QList<double> &elements)
{
    if(weight.size() != elements.size())
    {
        qFatal("Can not calculate weighted sum, if weight.size and elemnt.size do not match");
    }
    double weightedSum = 0;
    for(int i=0; i<elements.size(); i++)
    {
        weightedSum += elements.at(i) * weight.at(i);
    }
    return weightedSum;
}
