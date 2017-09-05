#include "logdatabase.h"

logDatabase::logDatabase()
{

}

void logDatabase::writeInteractionLog(interactionLog log)
{
    interactionLogDatabase.append(log);
}

interactionLog logDatabase::getInteractionLog(int position)
{
    if(interactionLogDatabase.size() > position)
    {
        return interactionLogDatabase.at(position);
    }
    else
    {
        qFatal("The log does not exist");
        return interactionLogDatabase.at(0);
    }
}

int logDatabase::sizeInteractionLogDatabase()
{
    return interactionLogDatabase.size();
}
