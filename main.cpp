#include "mainwindow.h"
#include "interaction.h"
#include <QApplication>
#include "randomQ.cpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    randomQ TruthRand(1000);

    interaction event(TruthRand);


    return a.exec();
}


