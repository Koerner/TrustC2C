#ifndef RANDOMQ_H
#define RANDOMQ_H

#include <random>
#include <QDebug>

class randomQ
{
public:
    randomQ();
    randomQ(int seed);

    void seedRandom(int seed);

    bool getBool();
    int getPercent();
    bool getResultPercent(int sucessPercent);


    QVector<unsigned long int> getCarID(unsigned long int totalCarNumber, int requiredIDs);

private:
    std::mt19937 randMT;

};


#endif // RANDOMQ_H
