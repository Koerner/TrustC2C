#ifndef RANDOMQ_H
#define RANDOMQ_H

#include <random>

class randomQ
{
public:
    randomQ(int seed);

    bool getBool();

private:
    std::mt19937 boolMT;

};


#endif // RANDOMQ_H
