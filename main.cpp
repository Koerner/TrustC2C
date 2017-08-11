#include "mainwindow.h"
#include "interaction.h"
#include <QApplication>
#include "randomQ.cpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //QVector<randomQ&> randomVector;
    randomQ TruthRand(1000); //new random generator for the truth of the situation in the enviroment
    //randomVector.append(&TruthRand);
    randomQ DetectRand(999); //new random generator for detecting the truth
    //randomVector.append(&DetectRand);
    randomQ HonestRand(5); //new random generator for detecting the truth
    //randomVector.append(&HonestRand);

    QVector<int> settingsVector;
    settingsVector.append(3); // number of interactions
    settingsVector.append(90); // probability that car detects trueth in percent
    settingsVector.append(50); // probability that car tells the truth to it's best knowledge

    qDebug() << "number of interactions" << settingsVector.at(0);

    //vector<interaction> vectorOfInteraction;
    for (int i=0; i<settingsVector.at(0); ++i)
    {
      interaction event(settingsVector, TruthRand, DetectRand, HonestRand);
      //vectorOfInteraction.push_back(event);
      event.run();
    }

    return a.exec();
}


