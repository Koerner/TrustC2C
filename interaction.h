#ifndef INTERACTION_H
#define INTERACTION_H

#include <random>
#include <iostream>
#include <QtDebug>
#include "randomQ.h"


class interaction
{
public:
    interaction(randomQ& truthRand);

private:
    void initTruth();
    void run();

    bool truth;


};

#endif // INTERACTION_H
