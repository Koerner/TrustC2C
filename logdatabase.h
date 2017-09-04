#ifndef LOGDATABASE_H
#define LOGDATABASE_H

#include "structs.h"


class logDatabase
{
public:
    logDatabase();
    writeInteractionLog(interactionLog log);
    interactionLog getInteractionLog(int position);
    int sizeInteractionLogDatabase();

private:
    QVector <interactionLog> interactionLogDatabase;


};

#endif // LOGDATABASE_H
