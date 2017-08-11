#ifndef RANDOMQ_H
#define RANDOMQ_H

#include <random>
#include <QDebug>

class randomQ
{
public:
    randomQ(int seed);

    bool getBool();
    int getPercent();
    bool getResultPercent(int sucessPercent);

private:
    std::mt19937 boolMT;

};


#endif // RANDOMQ_H
