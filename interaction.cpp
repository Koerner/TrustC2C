#include "interaction.h"


interaction::interaction(randomQ& truthRand)
{
    for(int i=0; i<10; i++)
    {
        truth = truthRand.getBool();
        qDebug() << "The reality is" << truth;
    }

}

void interaction::initTruth()
{


}

void interaction::run()
{

}

