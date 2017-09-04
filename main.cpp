#include "mainwindow.h"
#include "interaction.h"
#include <QApplication>
#include "randomQ.cpp"
#include "database.h"
#include "structs.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();





    // Setup Randoms

    randStruct randCollection;
    randCollection.truthRand.seedRandom(21035219);
    randCollection.detectRandA.seedRandom(73263905);
    randCollection.detectRandX.seedRandom(60485875);
    randCollection.honestRand.seedRandom(52006881);
    randCollection.carSelectRand.seedRandom(84934424);
    randCollection.carB2SelectRand.seedRandom(84934424);


    //Get GUI input


    //settings
    settingsGUI instanceSettings;
    instanceSettings.numInteractions = 5;
    instanceSettings.numTotalCars = 7; //max 2147483647 because of Qvector/Qlist (2 Billion) //  int unsigned long: 4294967296 (4 biilion)
    instanceSettings.numCarsRecommending = 5; //has to be -2 total cars

    // Propability distribution
    QList<QPair<int,int>> PropDetectsCarX;
    PropDetectsCarX.clear();
    PropDetectsCarX.append(QPair<unsigned int,int>(0 , 95));
    PropDetectsCarX.append(QPair<unsigned int,int>(instanceSettings.numTotalCars * 90 /100, 5));
    instanceSettings.PropDetectsCarX = PropDetectsCarX;
    qDebug() << PropDetectsCarX;

    // Propability honest  /// not yet implemented
    QList<QPair<int,int>> PropHonestCarB;
    PropHonestCarB.clear();
    PropHonestCarB.append(QPair<unsigned int,int>(100 / 100 * instanceSettings.numTotalCars , 90));
    PropHonestCarB.append(QPair<unsigned int,int>(instanceSettings.numTotalCars - 0 / 100 * instanceSettings.numTotalCars, 10));
    instanceSettings.PropHonestCarB = PropHonestCarB;

    instanceSettings.PropHonestCarX = 100;

    //trust chain specification
    /// be careful needs of lot of computing time
    instanceSettings.minRecomendingWidthDirect = 1; //minimal 1
    instanceSettings.minRecomendingWidth = 5; //minimal 1
    instanceSettings.maxRecomendingDepth = 5;  //Minimal 1

    //end settings


    //initilaize database with all cars
    database data(instanceSettings.numTotalCars, instanceSettings.PropDetectsCarX, instanceSettings.PropHonestCarB);
    logDatabase logdata;


    //generate interactions
    qDebug() << "number of interactions" << instanceSettings.numInteractions;
    qWarning() << "!!!!!!!!!!!!!! Start with interactions !!!!!!!!!!!!!!!!!!!!!";

    for (unsigned long int i=0; i < ceil((double)instanceSettings.numInteractions/100); i++)
    {
        for (unsigned long int j=0; j< 100; j++)
        {
        interaction event(instanceSettings, randCollection, data, logdata);
        event.run();
        }
        if(i%10 == 0)
        {
        w.setProgressBar(i * 100 * 100 / instanceSettings.numInteractions);
        w.setProgressBarCar1(data.getAverageTrust(0,1));
        w.setProgressBarCar2(data.getAverageTrust(0,instanceSettings.numTotalCars - 1));
        qApp->processEvents();
        }
    }
    qWarning() << "!!!!!!!!!!!!!!!!!!!! Done with interactions !!!!!!!!!!!!!!!!!!!!!!!!!!!!!";

    data.saveToFileTrust();
    data.saveToFileReputation();



    return a.exec();
}




