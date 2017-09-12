#include "mainwindow.h"
#include "interaction.h"
#include <QApplication>
#include "randomQ.cpp"
#include "database.h"
#include "structs.h"
#include <QtWidgets/QApplication>
#include "guiChart.h"
#include <QDesktopWidget>


int main(int argc, char *argv[])
{



 #define CERTAINTY_X_ON true

    //Get GUI Settings

    //settings
    settingsGUI instanceSettings;
    instanceSettings.numInteractionsFirst = 3000;
    instanceSettings.numTotalCarsFirst = 10; //max 2147483647 because of Qvector/Qlist (2 Billion) //  int unsigned long: 4294967296 (4 biilion)
     //max 2147483647 because of Qvector/Qlist (2 Billion) //  int unsigned long: 4294967296 (4 biilion)
    instanceSettings.numCarsRecommending = 5; //has to be -2 total cars
    instanceSettings.certaintyXon = true;

    //trust chain specification
    instanceSettings.minRecomendingWidthDirect = 1; //minimal 1
    instanceSettings.minRecomendingWidth = 5; //minimal 1
    instanceSettings.maxRecomendingDepth = 5;  //Minimal 1

    //std deviation
    double std_deviationObservation = 0.05;
    double std_deviationPrediction = 0.05;
    double std_deviationHonest = 0;

    // Propability for cars to detect the truth before arriving
    QList<QPair<int,QPair<double, double>>> PropDetectsPrediction;
    PropDetectsPrediction.clear();
    PropDetectsPrediction.append(QPair<unsigned int,QPair <double, double>>(0 , qMakePair(0.4, std_deviationPrediction)));
    PropDetectsPrediction.append(QPair<unsigned int,QPair <double, double>>(instanceSettings.numTotalCarsFirst * 90 /100, qMakePair(0.2, std_deviationPrediction)));
    instanceSettings.PropDetectsPrediction = PropDetectsPrediction;
    qDebug() << PropDetectsPrediction;

    // Propability for cars to detect the truth after situation (observation)
    QList<QPair<int,QPair<double, double>>> PropDetectsObservation;
    PropDetectsObservation.clear();
    PropDetectsObservation.append(QPair<unsigned int,QPair <double, double>>(0 , qMakePair(0.9, std_deviationObservation)));
    PropDetectsObservation.append(QPair<unsigned int,QPair <double, double>>(instanceSettings.numTotalCarsFirst * 50 /100, qMakePair(0.3, std_deviationObservation)));
    instanceSettings.PropDetectsObservation = PropDetectsObservation;
    qDebug() << PropDetectsObservation;

    // Propability honest  /// not yet implemented
    QList<QPair<int,QPair<double, double>>> PropHonestCarX;
    PropHonestCarX.clear();
    PropHonestCarX.append(QPair<unsigned int,QPair <double, double>>(0 , qMakePair(1.0, std_deviationHonest)));
    PropHonestCarX.append(QPair<unsigned int,QPair <double, double>>(instanceSettings.numTotalCarsFirst * 100 /100, qMakePair(0.1, std_deviationHonest)));
    instanceSettings.PropHonestCarX = PropHonestCarX;

    //instanceSettings.PropHonestCarX = 1;



    //Batch II
    QPair<double, double> PropDetectsPredictionNew = qMakePair(0.30, std_deviationPrediction);
    QPair<double, double> PropDetectsObservationNew = qMakePair(0.95, std_deviationObservation);
    QPair<double, double> PropHonestNew = qMakePair(1.0, std_deviationHonest);
    instanceSettings.numInteractionsSecond = 180;
    instanceSettings.numTotalCarsSecond = instanceSettings.numTotalCarsFirst + 1; //do not change

    //end settings

    // Setup Randoms



    QList<logDatabase> logdataList;
    QList<randStruct> randList;
    QList<database> databaseList;

    for(int k=0; k<4; k++)
    {
        qWarning() << "Start run" << k+1;

        //rand Init
        randStruct randCollection;
        randCollection.truthRand.seedRandom(21035219+k*13);
        randCollection.detectRandA.seedRandom(73263905+k*11);
        randCollection.detectRandX.seedRandom(604858754+k*7);
        randCollection.honestRand.seedRandom(52006881+k*3);
        randCollection.carSelectRandAX.seedRandom(84934424+k*5);
        randCollection.carSelectRandB.seedRandom(84934424+k*17);
        randCollection.carB2SelectRand.seedRandom(84934424+k*19);
        randCollection.Poison.seedRandom(67934424+k*2);

        randList.append(randCollection);

        //database init
        database data(instanceSettings.numTotalCarsFirst, instanceSettings.PropDetectsPrediction, instanceSettings.PropDetectsObservation, instanceSettings.PropHonestCarX);
        databaseList.append(data);
        logDatabase logdata;
        logdataList.append(logdata);

        //interaction batch I

        for (unsigned long int i=0; i < ceil((double)instanceSettings.numInteractionsFirst/100); i++)
        {
            for (unsigned long int j=0; j< 100; j++)
            {
            interaction event(instanceSettings, randList[k], databaseList[k], logdataList[k]);
            event.run();
            }
            if(i%10 == 0)
            {
            qApp->processEvents();
            }
        }

        //interaction batch II
        databaseList[k].addCar(PropDetectsPredictionNew, PropDetectsObservationNew, PropHonestNew);

        for (unsigned long int i=0; i < ceil((double)instanceSettings.numInteractionsSecond/100); i++)
        {
            for (unsigned long int j=0; j< 100; j++)
            {
            interaction event(instanceSettings, randList[k], databaseList[k], logdataList[k]);
            event.run();
            }
            if(i%10 == 0)
            {
            qApp->processEvents();
            }
        }

        qWarning() << "End run" << k+1;


    }



    /******************************************
     *  Display 4 graphs
     * **************************************/

    int interactionFirst = instanceSettings.numInteractionsFirst -180 + instanceSettings.numInteractionsSecond;
    int interactionLast =instanceSettings.numInteractionsFirst + instanceSettings.numInteractionsSecond;
    unsigned long int showCarID = 0; //instanceSettings.numTotalCarsSecond -5;

    QApplication a(argc, argv);
    QDesktopWidget dwO;
    QRect dw = dwO.screenGeometry(0);

    //MainWindow w;
    //w.show();
    //w.createChart(logdataList,databaseList, 0, instanceSettings.numTotalCarsSecond-1, carIDfirst, carIDsecond);
    //QApplication b(argc, argv);



    guiChart window1(logdataList[0], databaseList[0], showCarID, interactionFirst, interactionLast);
    window1.setWindowTitle("Run I");
    window1.move(QPoint(dw.width()*0,dw.height()*0));
    window1.resize(QSize(dw.width()*0.49, dw.height()*0.45));
    window1.show();

    guiChart window2(logdataList[1], databaseList[1], showCarID, interactionFirst, interactionLast);
    window2.setWindowTitle("Run II");
    window2.move(QPoint(dw.width()*0.5,dw.height()*0));
    window2.resize(QSize(dw.width()*0.49, dw.height()*0.45));
    window2.show();

    guiChart window3(logdataList[2], databaseList[2], showCarID, interactionFirst, interactionLast);
    window3.setWindowTitle("Run III");
    window3.move(QPoint(dw.width()*0,dw.height()*0.47));
    window3.resize(QSize(dw.width()*0.49, dw.height()*0.45));
    window3.show();

    guiChart window4(logdataList[3], databaseList[3], showCarID, interactionFirst, interactionLast);
    window4.setWindowTitle("Run IV");
    window4.move(QPoint(dw.width()*0.5,dw.height()*0.47));
    window4.resize(QSize(dw.width()*0.49, dw.height()*0.45));
    window4.show();

    /******************************************
     *  Auswertung
     * **************************************/




    return a.exec();
}




