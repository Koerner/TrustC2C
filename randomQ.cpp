#include "randomQ.h"


randomQ::randomQ(int seed)
{
    boolMT.seed(seed);

}

bool randomQ::getBool()
{
    std::uniform_int_distribution<int> boolDist(0, 1);
    return boolDist(boolMT);
}



