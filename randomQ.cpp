#include "randomQ.h"

randomQ::randomQ()
{
    /// randomQ constructor, input: seed.
    /** seeds the std::mt19937 random generator with an <int>. */

    randMT.seed(0);

}


randomQ::randomQ(int seed)
{
    /// randomQ constructor, input: seed.
    /** seeds the std::mt19937 random generator with an <int>. */

    randMT.seed(seed);

}

void randomQ::seedRandom(int seed)
{
    randMT.seed(seed);
}


bool randomQ::getBool()
{
    /// returns random bool.
    /** Returns one random bool from an uniform distribution based on std::mt19937. */

    std::uniform_int_distribution<int> boolDist(0, 1);
    return boolDist(randMT);
}

double randomQ::getPoisonPercent(double mean, double stddiviation)
{
    /// returns random bool.
    /** Returns one random bool from an uniform distribution based on std::mt19937. */

    std::normal_distribution<double> normalDist(mean,stddiviation);

    double percent = normalDist(randMT);

    if(percent>=1.0)
    {
        percent = 0.9999;
    }
    if(percent<=0)
    {
        percent = 0.0001;
    }
    double returnValue = percent;

    return returnValue;
}

int randomQ::getPercent()
{
    /// returns random <int> between 0 and 99.
    /** Returns one random <int> between 0 and 99, from an uniform distribution based on std::mt19937. */

    std::uniform_int_distribution<int> intDist(0, 99);
    return intDist(randMT);
}

bool randomQ::getResultPercent(double sucessPercent)
{
    /// returns random succes for a event with a probability in percent. input: percent for success.
    /**  */
    //qDebug() << "Car %:" << sucessPercent;

    if(sucessPercent > 1.000)
    {
        qFatal("percentage can not be higher than 100 or lower than 0: %f", sucessPercent);
    }
    double tempPercent = sucessPercent * 100;
    if(getPercent() < tempPercent)
    {
        return true;
    }
    else
    {
        return false;
    }
}

QVector<unsigned long int> randomQ::getCarID(unsigned long int totalCarNumber, int requiredIDs, QVector<unsigned long> blockedCars)
{
    /// returns random succes for a event with a probability in percent. input: percent for success.
    /**  */

    std::uniform_int_distribution<unsigned long int> longDist(0, totalCarNumber - 1);

    QVector<unsigned long int> carIDs;
    carIDs.clear();

    for(int i=0; i<requiredIDs; i++)
    {
        int temp = longDist(randMT);
        //qDebug() << "Car ID: " << temp;

        if(carIDs.contains(temp) || blockedCars.contains(temp))
        {
            //we never want to have the same carIDs in one batch!
            i--;
            //qDebug() << "Detected same CarID in one batch, generating a new one";
        }
        else
        {
            carIDs.append(temp);
        }
    }

    //SaveCheck
    if(carIDs.size()==requiredIDs)
    {
        return carIDs;
    }
    else
    {
        qFatal("Somehow the required number of IDs does not match the size of the ID vector");
    }

}






