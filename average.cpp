#include "average.h"

average::average()
{

}

double average::averageMean(QList<double> &elements)
{
    if(elements.size() == 0)
    {
        return 0;
    }
    else if(elements.size() == 1)
    {
        return elements.at(0);
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
            return 0;
        }
        else
        {
            return sum /  counter;
        }

    }
}
