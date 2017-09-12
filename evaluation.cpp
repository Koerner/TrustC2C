#include "evaluation.h"

evaluation::evaluation(QList<logDatabase> &logDataHandover)
{
    logData = &logDataHandover;
}

void evaluation::rightDecission(int run, int firstInteraction, int lastInteraction, unsigned long firstCar, unsigned long lastCar)
{
    logDatabase currentLog = logData->at(run);


    ResultPercentageByCar.clear();
    APercentageByCar.clear();
    XPercentageByCar.clear();
    MaxPercentageByCar.clear();


    for(unsigned long int i=firstCar; i<=lastCar; i++)
    {
        double sumResult = 0;
        double sumA = 0;
        double sumX = 0;
        double sumMax = 0;
        int counter = 0;
        for(int j=firstInteraction; j<=lastInteraction; j++)
        {
            if(currentLog.getInteractionLog(j).carIDs.at(0) == i)
            {
                sumResult += currentLog.getInteractionLog(j).correctDecission;
                bool A = currentLog.getInteractionLog(j).carAprediction.first == currentLog.getInteractionLog(j).truth;
                sumA += A;
                bool X = currentLog.getInteractionLog(j).carXsays.first == currentLog.getInteractionLog(j).truth;
                sumX += X;
                sumMax += std::max(A, X);
                counter++;
            }
        }
        ResultPercentageByCar.append(sumResult/counter);
        APercentageByCar.append(sumA/counter);
        XPercentageByCar.append(sumX/counter);
        MaxPercentageByCar.append(sumMax/counter);


    }

    QString filename;
    filename.append("C:\\Users\\Quirin\\Desktop\\Evaluation");
    filename.append(QString::number(run));
    filename.append(".txt");
    writeToFile(ResultPercentageByCar, APercentageByCar, XPercentageByCar, MaxPercentageByCar, filename);



}

void evaluation::writeToFile(QList<double> ResultPercentageByCar, QList<double> APercentageByCar, QList<double> XPercentageByCar, QList<double> MaxPercentageByCar, QString filename)
{

    QFile file( filename );
    if ( file.open(QIODevice::WriteOnly) )
    {
        QTextStream stream( &file );

        for(int i=0; i < ResultPercentageByCar.size(); i++)
        {
            stream << "Car" << i << ";"
                   << ResultPercentageByCar.at(i) << ";"
                      << APercentageByCar.at(i) << ";"
                         << XPercentageByCar.at(i) << ";"
                            << MaxPercentageByCar.at(i) << ";"
                   <<"\r\n";
        }
        stream << "Ende" << endl;
    }
}

void evaluation::showChart()
{
    QT_CHARTS_USE_NAMESPACE
    //QList<QBarSet*> QBarList;
    QBarSeries *series = new QBarSeries();

    /*
    for(int i=0; i<ResultPercentageByCar.size(); i++)
    {
        QString name = "Car";
        name.append(QString::number(i));
        QBarList.append(new QBarSet(name));
        //QBarList[i] = new QBarSet("Car 0");
        //set.setObjectName("Car 0");
        *QBarList[i] << ResultPercentageByCar.at(0) << APercentageByCar.at(0) << XPercentageByCar.at(0) << MaxPercentageByCar.at(0);


        series->append(QBarList.at(i));
    }
    */

    QBarSet *setResult = new QBarSet("Result");
    for(int i=0; i<ResultPercentageByCar.size(); i++)
    {
        *setResult << ResultPercentageByCar.at(i);
    }
    series->append(setResult);

    QBarSet *setA = new QBarSet("Car A");
    for(int i=0; i<ResultPercentageByCar.size(); i++)
    {
        *setA << APercentageByCar.at(i);
    }
    series->append(setA);

    QBarSet *setX = new QBarSet("Car X");
    for(int i=0; i<ResultPercentageByCar.size(); i++)
    {
        *setX << XPercentageByCar.at(i);
    }
    series->append(setX);

    QBarSet *setMax = new QBarSet("Car max");
    for(int i=0; i<MaxPercentageByCar.size(); i++)
    {
        *setMax << MaxPercentageByCar.at(i);
    }
    series->append(setMax);


    QStringList categories;
    for(int i=0; i<MaxPercentageByCar.size(); i++)
    {
        QString name ="Car ";
        name.append(QString::number(i));
        categories << name;
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);


        QBarCategoryAxis *axis = new QBarCategoryAxis();
        axis->append(categories);
        chart->createDefaultAxes();
        chart->setAxisX(axis, series);

        chart->axisY()->setMax(1.0);

        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);

        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        window.setCentralWidget(chartView);

}

QList<double> evaluation::average(QList<QList<double>> ListList)
{
    QList<double> List;
    for(int i=0; i<ListList.size(); i++)
    {
        List +=ListList.at(i);
    }
    return List;
}

