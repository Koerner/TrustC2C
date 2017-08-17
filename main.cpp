#include "mainwindow.h"
#include "interaction.h"
#include <QApplication>
#include "randomQ.cpp"
#include "database.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // Setup Timers

    //QVector<randomQ&> randomVector;
    randomQ TruthRand(1000); //new random generator for the truth of the situation in the enviroment
    //randomVector.append(&TruthRand);
    randomQ DetectRandX(999); //new random generator for detecting the truth
    //randomVector.append(&DetectRand);
    randomQ HonestRand(5); //new random generator for detecting the truth
    //randomVector.append(&HonestRand);
    randomQ DetectRandA(100); //new random generator for detecting the truth
    //randomVector.append(&HonestRand);

    //Get GUI input
    QVector<int> settingsVector;
    settingsVector.append(3); // number of interactions
    settingsVector.append(90); // probability that carX detects trueth in percent
    settingsVector.append(50); // probability that carX tells the truth to it's best knowledge
    settingsVector.append(90); // probability that carA tells the truth to it's best knowledge

    //initilaize database with all cars
    database data(100);


    //TEST
    data.writeTrustX(1,10,9.0);
    data.writeTrustX(1,10,3.5);
    qDebug() << "Test: " << data.getCarTrust(1,10);





    //generate interactions

    qDebug() << "number of interactions" << settingsVector.at(0);

    //vector<interaction> vectorOfInteraction;
    for (int i=0; i<settingsVector.at(0); ++i)
    {
      interaction event(settingsVector, TruthRand, DetectRandX, HonestRand, DetectRandA);
      //vectorOfInteraction.push_back(event);
      event.run();
    }

    return a.exec();
}




