#include "mainwindow.h"
#include "interaction.h"
#include <QApplication>
#include "randomQ.cpp"
#include "database.h"
#include "structs.h"
#include <QtWidgets/QApplication>
#include "guiChart.h"
#include <QDesktopWidget>
#include "evaluation.h"
#include <windows.h>

int main(int argc, char *argv[])
{



 #define CERTAINTY_X_ON true

    //Get GUI Settings

    //settings
    settingsGUI instanceSettings;

    /******************************
     * Settings for parameters of the cars
     * **************************/

    //std deviations for probabilities below
    double std_deviationObservation = 0.05;
    double std_deviationPrediction = 0.05;
    double std_deviationX = 0.05;
    double std_deviationHonest = 0.05;

    // Propability for cars to detect the truth before arriving
    QList<QPair<unsigned long int,QPair<double, double>>> PropDetectsPrediction;
    PropDetectsPrediction.clear();
    PropDetectsPrediction.append(QPair<unsigned long int,QPair <double, double>>(0 , qMakePair(0.50, std_deviationPrediction)));
    PropDetectsPrediction.append(QPair<unsigned long int,QPair <double, double>>(instanceSettings.numTotalCarsFirst * 50 /100, qMakePair(0.2, std_deviationPrediction)));
    instanceSettings.PropDetectsPrediction = PropDetectsPrediction;
    qDebug() << PropDetectsPrediction;

    // Propability for for X to detect the truth
    QList<QPair<unsigned long int,QPair<double, double>>> PropDetectsX;
    PropDetectsX.clear();
    PropDetectsX.append(QPair<unsigned long int,QPair <double, double>>(0 , qMakePair(0.95, std_deviationX)));
    PropDetectsX.append(QPair<unsigned long int,QPair <double, double>>(instanceSettings.numTotalCarsFirst * 80 /100, qMakePair(0.60, std_deviationX)));
    instanceSettings.PropDetectsX = PropDetectsX;
    qDebug() << PropDetectsX;

    // Propability for cars to detect the truth after situation (observation)
    QList<QPair<unsigned long int,QPair<double, double>>> PropDetectsObservation;
    PropDetectsObservation.clear();
    PropDetectsObservation.append(QPair<unsigned long int,QPair <double, double>>(0 , qMakePair(1.0, std_deviationObservation)));
    PropDetectsObservation.append(QPair<unsigned long int,QPair <double, double>>(instanceSettings.numTotalCarsFirst * 100 /100, qMakePair(1.0, std_deviationObservation)));
    instanceSettings.PropDetectsObservation = PropDetectsObservation;
    qDebug() << PropDetectsObservation;

    // Propability honest for X
    QList<QPair<unsigned long int,QPair<double, double>>> PropHonestCarX;
    PropHonestCarX.clear();
    PropHonestCarX.append(QPair<unsigned long int,QPair <double, double>>(0 , qMakePair(0.01, std_deviationHonest)));
    PropHonestCarX.append(QPair<unsigned long int,QPair <double, double>>(instanceSettings.numTotalCarsFirst * 40 /100, qMakePair(1.0, std_deviationHonest)));
    instanceSettings.PropHonestCarX = PropHonestCarX;

    QList<database> databaseList;
    //The paramters for the cars are saved to the database. From this point on all varaiibles from above are fixed and can't be changed

    /*******************************
     * Interaction specific settings
     * ****************************/

    //Recommendation Parameters
    instanceSettings.numCarsRecommending = 5; //has to be -2 total cars
    instanceSettings.maxLevelWidth = 5; //minimal 1
    instanceSettings.maxRecomendingDepth = 3;  //Minimal 1

    //Allow X to send certainty
    instanceSettings.certaintyXon = true;

    /*******************
     * Batch I
     * *****************/

    instanceSettings.numTotalCarsFirst = 600; //max 2147483647 because of Qvector/Qlist (2 Billion) //  int unsigned long: 4294967296 (4 biilion)
    //instanceSettings.numInteractionsFirst = 100*100*0.5; //High density numCar²*30 | Low density numCar²*1
    instanceSettings.numInteractionsFirst = std::pow(instanceSettings.numTotalCarsFirst,2) * 0.25; //density factor


    /*******************
     * Batch II
     * the second batch introduces a new car without any trust values (from and to)
     * *****************/

    //parameters for new car
    QPair<double, double> PropDetectsPredictionNew = qMakePair(0.4, std_deviationPrediction);
    QPair<double, double> PropDetectsObservationNew = qMakePair(0.95, std_deviationObservation);
    QPair<double, double> PropHonestNew = qMakePair(1.0, std_deviationHonest);

    //interaction quantity batch II
    instanceSettings.numInteractionsSecond = 180;
    instanceSettings.numTotalCarsSecond = instanceSettings.numTotalCarsFirst + 1; //do not change

    //end settings

    QList<logDatabase> logdataList; //generates database for log
    QList<randStruct> randList; // to store all seeded random generators
    QElapsedTimer timer;





    /***************
     * Specifies and executes all for runs. The number of runs is fixed to forur.
     * ************/

    //Random seed multiplier, specifies if and how the seed of the randomes should change
    // if 0 => all four runns will have the exact same seeding for the random generators
    int randomeMulti = 1;

    timer.start();
    for(int k=0; k<4; k++)
    {
        timer.restart();

        switch(k){
        /***************
         * Setting, which change in each run. Add variables you want to change (only "Interaction specific settings")
         * ************/
        case 0:
            instanceSettings.numCarsRecommending = 5;
            instanceSettings.maxRecomendingDepth = 1;
            break;
        case 1:
            instanceSettings.numCarsRecommending = 5;
            instanceSettings.maxRecomendingDepth = 2;
            break;
        case 2:
            instanceSettings.numCarsRecommending = 5;
            instanceSettings.maxRecomendingDepth = 3;
            break;
        case 3:
            instanceSettings.numCarsRecommending = 5;
            instanceSettings.maxRecomendingDepth = 5;
            break;
        }

        qWarning() << "Start run" << k+1;

        //Seed all necessary random generators
        randStruct randCollection;
        randCollection.truthRand.seedRandom(21035219+k*13*randomeMulti); //to determine the truth
        randCollection.detectRandA.seedRandom(73263905+k*11*randomeMulti); //for the detection of A
        randCollection.detectRandX.seedRandom(604858754+k*7*randomeMulti); //for the detection of X
        randCollection.honestRand.seedRandom(52006881+k*3*randomeMulti); //to determine if X is honest
        randCollection.carSelectRandAX.seedRandom(84934424+k*5*randomeMulti); //to determine car A and X in each interaction
        randCollection.carSelectRandB.seedRandom(84934424+k*17*randomeMulti); //to determine car Bs in each interaction
        randCollection.carB2SelectRand.seedRandom(84934424+k*19*randomeMulti); // not in use
        randCollection.Poison.seedRandom(67934424+k*2*randomeMulti); //randome to add a gausian normal distribution for more realistic results
        randList.append(randCollection);

        //database init
        database data(instanceSettings.numTotalCarsFirst, instanceSettings.PropDetectsPrediction, instanceSettings.PropDetectsObservation, instanceSettings.PropDetectsX, instanceSettings.PropHonestCarX);
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

        qWarning() << "End run" << k+1 << "Elapsed time: " << timer.elapsed();


    }

    /******************************************
     *  Display graphs and save to file
     * **************************************/

    QApplication a(argc, argv);
    QDesktopWidget dwO;
    QRect dw = dwO.screenGeometry(0);

    /******************************************
     *  Settings for all graphs an evaluation
     * **************************************/

    int interactionFirst = 0; //instanceSettings.numTotalCarsFirst; // to exclude very first results
    int interactionLast = instanceSettings.numInteractionsFirst;//instanceSettings.numInteractionsFirst + instanceSettings.numInteractionsSecond;

    /******************************************
     *  Settings for the four single interaction graphs and table
     * **************************************/

    unsigned long int showCarID = 0; //instanceSettings.numTotalCarsSecond -5;

    /*****************************************/

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
     *  Evaluation bar charts and summary bar chart
     * **************************************/

    int carDisplayFirst = 0; //startCar
    int carDisplayLast = instanceSettings.numTotalCarsFirst -1; //EndCarID
    int numDisplayCars = 10; //Single values displayed besides average

    /*****************************************/

    evaluation eval0(logdataList);
    eval0.rightDecission(0,interactionFirst,interactionLast-1,carDisplayFirst, carDisplayLast);
    eval0.showChart(numDisplayCars);
    eval0.window.move(QPoint(dw.width()*0,dw.height()*0));
    eval0.window.resize(QSize(dw.width()*0.49, dw.height()*0.45));
    eval0.window.setWindowTitle("Run_I");
    eval0.window.show();

    evaluation eval1(logdataList);
    eval1.rightDecission(1,interactionFirst,interactionLast-1,carDisplayFirst,carDisplayLast);
    eval1.showChart(numDisplayCars);
    eval1.window.move(QPoint(dw.width()*0.5,dw.height()*0));
    eval1.window.resize(QSize(dw.width()*0.49, dw.height()*0.45));
    eval1.window.setWindowTitle("Run_II");
    eval1.window.show();

    evaluation eval2(logdataList);
    eval2.rightDecission(2,interactionFirst,interactionLast-1,carDisplayFirst,carDisplayLast);
    eval2.showChart(numDisplayCars);
    eval2.window.move(QPoint(dw.width()*0,dw.height()*0.47));
    eval2.window.resize(QSize(dw.width()*0.49, dw.height()*0.45));
    eval2.window.setWindowTitle("Run_III");
    eval2.window.show();

    evaluation eval3(logdataList);
    eval3.rightDecission(3,interactionFirst,interactionLast-1,carDisplayFirst,carDisplayLast);
    eval3.showChart(numDisplayCars);
    eval3.window.move(QPoint(dw.width()*0.5,dw.height()*0.47));
    eval3.window.resize(QSize(dw.width()*0.49, dw.height()*0.45));
    eval3.window.setWindowTitle("Run_IV");
    eval3.window.show();

    evaluation eval4(logdataList);
    eval4.showChartOutcome(eval0.ResultPercentageByCar, eval1.ResultPercentageByCar, eval2.ResultPercentageByCar, eval3.ResultPercentageByCar, eval3.MaxPercentageByCar, 10);
    eval4.window.move(QPoint(dw.width()*1.1,dw.height()*0.47));
    //eval4.window.setMaximumSize();   resize(QSize(dw.width()*0.49, dw.height()*0.45));
    eval4.window.setWindowTitle("ResultComparisonBetweenRuns");
    eval4.window.showFullScreen();


    //Save Graphics

    ///TODO bugfix, not nice
    for(int z=0; z<50;z++) //sleeper to wait for the graph animations
    {
        Sleep(10);
        qApp->processEvents();
    }

    QString location = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);  //folder -> Desktop
    QString file = location;
    file.append("/");
    file.append(eval0.window.windowTitle());
    file.append(".png");
    eval0.window.grab().save(file);

    file = location;
    file.append("/");
    file.append(eval1.window.windowTitle());
    file.append(".png");
    eval1.window.grab().save(file);

    file = location;
    file.append("/");
    file.append(eval2.window.windowTitle());
    file.append(".png");
    eval2.window.grab().save(file);

    file = location;
    file.append("/");
    file.append(eval3.window.windowTitle());
    file.append(".png");
    eval3.window.grab().save(file);

    file = location;
    file.append("/");
    file.append(eval4.window.windowTitle());
    file.append(".png");
    eval4.window.grab().save(file);

    qWarning() << "Saved images to " << file;



    return a.exec();
}




