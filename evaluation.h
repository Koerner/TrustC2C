#ifndef EVALUATION_H
#define EVALUATION_H

#include "logdatabase.h"


class evaluation
{
public:
    evaluation(logDatabase& logDataHandover);
private:
    logDatabase *logData;
};

#endif // EVALUATION_H
