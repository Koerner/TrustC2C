#include "randomQ.h"


randomQ::randomQ(int seed)
{
    /// randomQ constructor, input: seed.
    /** seeds the std::mt19937 random generator with an <int>. */

    boolMT.seed(seed);

}

bool randomQ::getBool()
{
    /// returns random bool.
    /** Returns one random bool from an uniform distribution based on std::mt19937. */

    std::uniform_int_distribution<int> boolDist(0, 1);
    return boolDist(boolMT);
}

int randomQ::getPercent()
{
    /// returns random <int> between 0 and 99.
    /** Returns one random <int> between 0 and 99, from an uniform distribution based on std::mt19937. */

    std::uniform_int_distribution<int> intDist(0, 99);
    return intDist(boolMT);
}

bool randomQ::getResultPercent(int sucessPercent)
{
    /// returns random succes for a event with a probability in percent. input: percent for success.
    /**  */

    std::uniform_int_distribution<int> intDist(0, 99);
    int temp = intDist(boolMT);

    if(sucessPercent > 100)
    {
        qFatal("percentage can not be higher than 100 or lower than 0: %d", sucessPercent);
    }

    bool result = false;

    if(temp < sucessPercent)
    {
        result = true;
    }

    return result;
}



