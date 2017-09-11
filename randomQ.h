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
    bool getResultPercent(double sucessPercent);


    QVector<unsigned long int> getCarID(unsigned long int totalCarNumber, int requiredIDs, QVector<unsigned long int> blockedCars);

    double getPoison(double mean);
    double getPoisonPercent(double mean);
private:
    std::mt19937 randMT;

};


#endif // RANDOMQ_H
