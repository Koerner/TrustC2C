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


 #define CERTAINTY_X_ON true


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
    instanceSettings.numInteractions = 300;
    instanceSettings.numTotalCars = 10; //max 2147483647 because of Qvector/Qlist (2 Billion) //  int unsigned long: 4294967296 (4 biilion)
    instanceSettings.numCarsRecommending = 5; //has to be -2 total cars
    instanceSettings.certaintyXon = true;

    // Propability for cars to detect the truth before arriving
    QList<QPair<int,double>> PropDetectsPrediction;
    PropDetectsPrediction.clear();
    PropDetectsPrediction.append(QPair<unsigned int,double>(0 , 0.95));
    PropDetectsPrediction.append(QPair<unsigned int,double>(instanceSettings.numTotalCars * 90 /100, 0.05));
    instanceSettings.PropDetectsPrediction = PropDetectsPrediction;
    qDebug() << PropDetectsPrediction;

    // Propability for cars to detect the truth after situation (observation)
    QList<QPair<int,double>> PropDetectsObservation;
    PropDetectsObservation.clear();
    PropDetectsObservation.append(QPair<unsigned int,double>(0 , 0.95));
    PropDetectsObservation.append(QPair<unsigned int,double>(instanceSettings.numTotalCars * 90 /100, 0.05));
    instanceSettings.PropDetectsObservation = PropDetectsObservation;
    qDebug() << PropDetectsObservation;






    // Propability honest  /// not yet implemented
    QList<QPair<int,double>> PropHonestCarB;
    PropHonestCarB.clear();
    PropHonestCarB.append(QPair<unsigned int,double>(100 / 100 * instanceSettings.numTotalCars , 90));
    PropHonestCarB.append(QPair<unsigned int,double>(instanceSettings.numTotalCars - 0 / 100 * instanceSettings.numTotalCars, 10));
    instanceSettings.PropHonestCarB = PropHonestCarB;

    instanceSettings.PropHonestCarX = 100;

    //trust chain specification
    /// be careful needs of lot of computing time
    instanceSettings.minRecomendingWidthDirect = 1; //minimal 1
    instanceSettings.minRecomendingWidth = 5; //minimal 1
    instanceSettings.maxRecomendingDepth = 5;  //Minimal 1

    //end settings


    //initilaize database with all cars
    database data(instanceSettings.numTotalCars, instanceSettings.PropDetectsPrediction, instanceSettings.PropDetectsObservation, instanceSettings.PropHonestCarB);
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




