#include "mainwindow.h"
#include "interaction.h"
#include <QApplication>
#include "randomQ.cpp"
#include "database.h"
#include "structs.h"

#include <QTableView>
#include "logModel.h"


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
    instanceSettings.numInteractions = 100000;
    instanceSettings.numTotalCars = 20; //max 2147483647 because of Qvector/Qlist (2 Billion) //  int unsigned long: 4294967296 (4 biilion)
    instanceSettings.numCarsRecommending = 5; //has to be -2 total cars
    instanceSettings.numRecomendingDepth = 1;


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
    //end settings


    //initilaize database with all cars
    database data(instanceSettings.numTotalCars, instanceSettings.PropDetectsCarX, instanceSettings.PropHonestCarB);


    //generate interactions
    qDebug() << "number of interactions" << instanceSettings.numInteractions;
    qWarning() << "Start with interactions";

    for (unsigned long int i=0; i < instanceSettings.numInteractions/100; i++)
    {
        for (unsigned long int j=0; j< 100; j++)
        {
        interaction event(instanceSettings, randCollection, data);
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
    qWarning() << "Done with interactions";

    data.saveToFileTrust();
    data.saveToFileReputation();


    //Display results

    QTableView tableView;
    LogModel myModel(0, data, instanceSettings.numInteractions);
    tableView.setModel( &myModel );
    tableView.show();


    return a.exec();
}




