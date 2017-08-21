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
    randCollection.truthRand.seedRandom(1000);
    randCollection.detectRandA.seedRandom(5);
    randCollection.detectRandX.seedRandom(5);
    randCollection.honestRand.seedRandom(999);
    randCollection.carSelectRand.seedRandom(878);


    //Get GUI input


    //settings
    settingsGUI instanceSettings;
    instanceSettings.numInteractions = 1000;
    instanceSettings.numTotalCars = 10; //max 2147483647 because of Qvector/Qlist (2 Billion) //  int unsigned long: 4294967296 (4 biilion)
    instanceSettings.numCarsRecommending = 5; //has to be -2 total cars

    instanceSettings.PropDetectsCarA =90;

    // Propability distribution
    QList<QPair<int,int>> PropDetectsCarX;
    PropDetectsCarX.clear();
    PropDetectsCarX.append(QPair<unsigned int,int>(0 , 99));
    PropDetectsCarX.append(QPair<unsigned int,int>(instanceSettings.numTotalCars * 90 /100, 1));
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


    //TEST






    //generate interactions

    qDebug() << "number of interactions" << instanceSettings.numInteractions;

    //vector<interaction> vectorOfInteraction;
    for (unsigned long int i=0; i<instanceSettings.numInteractions; ++i)
    {
      interaction event(instanceSettings, randCollection, data);
      //vectorOfInteraction.push_back(event);
      event.run();
    }

    //Display results
    QTableView tableView;
    LogModel myModel(0, data, instanceSettings.numInteractions);
    tableView.setModel( &myModel );
    tableView.show();

    return a.exec();
}




