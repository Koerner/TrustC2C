#ifndef AVERAGE_H
#define AVERAGE_H

#include "QList"



class average
{
public:
    average();
    static QPair<double, int>  averageMean(QList<double> &elements);
    static double sum(QList<double> &elements);
    static double weightedSum(QList<double> &weight, QList<double> &elements);
};

#endif // AVERAGE_H
