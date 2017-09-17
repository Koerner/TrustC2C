#include "evaluation.h"
#include "QStandardPaths"

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
    MinPercentageByCar.clear();


    for(unsigned long int i=firstCar; i<=lastCar; i++)
    {
        double sumResult = 0;
        double sumA = 0;
        double sumX = 0;
        double sumMax = 0;
        double sumMin = 0;
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
                sumMin += std::min(A, X);
                counter++;
            }
        }
        ResultPercentageByCar.append(sumResult/counter);
        APercentageByCar.append(sumA/counter);
        XPercentageByCar.append(sumX/counter);
        MaxPercentageByCar.append(sumMax/counter);
        MinPercentageByCar.append(sumMin/counter);
        AvPercentageByCar.append((sumMin/counter+sumMax/counter)/2);


    }

    QString filename;
    filename.append(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    filename.append("/Evaluation");
    filename.append(QString::number(run));
    filename.append(".txt");
    writeToFile(ResultPercentageByCar, APercentageByCar, XPercentageByCar, MaxPercentageByCar, MinPercentageByCar, AvPercentageByCar, filename);
    qWarning() << "wrote files to " << filename;



}

void evaluation::writeToFile(QList<double> ResultPercentageByCar, QList<double> APercentageByCar, QList<double> XPercentageByCar, QList<double> MaxPercentageByCar, QList<double> MinPercentageByCar, QList<double> AvPercentageByCar, QString filename)
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
                               << MinPercentageByCar.at(i) << ";"
                                  << AvPercentageByCar.at(i) << ";"
                   <<"\r\n";
        }
        stream << "Ende" << endl;
    }
}

void evaluation::showChart(int numDisplayCars)
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
    for(int i=0; i<ResultPercentageByCar.size()&&i<numDisplayCars; i++)
    {
        *setResult << ResultPercentageByCar.at(i);
    }
    series->append(setResult);

    QBarSet *setA = new QBarSet("Car A");
    for(int i=0; i<ResultPercentageByCar.size()&&i<numDisplayCars; i++)
    {
        *setA << APercentageByCar.at(i);
    }
    series->append(setA);

    QBarSet *setX = new QBarSet("Car X");
    for(int i=0; i<ResultPercentageByCar.size()&&i<numDisplayCars; i++)
    {
        *setX << XPercentageByCar.at(i);
    }
    series->append(setX);

    QBarSet *setMax = new QBarSet("Car max");
    for(int i=0; i<MaxPercentageByCar.size()&&i<numDisplayCars; i++)
    {
        *setMax << MaxPercentageByCar.at(i);
    }
    series->append(setMax);

    QBarSet *setMin = new QBarSet("Car min");
    for(int i=0; i<MinPercentageByCar.size() && i<numDisplayCars; i++)
    {
        *setMin << MinPercentageByCar.at(i);
    }
    series->append(setMin);

    QBarSet *setAv = new QBarSet("Car Av");
    for(int i=0; i<AvPercentageByCar.size() && i<numDisplayCars; i++)
    {
        *setAv << AvPercentageByCar.at(i);
    }
    series->append(setAv);


    QStringList categories;
    for(int i=0; i<MaxPercentageByCar.size()&&i<numDisplayCars; i++)
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

void evaluation::showChartOutcome(QList<double> ResultPercentageByCar1, QList<double> ResultPercentageByCar2, QList<double> ResultPercentageByCar3, QList<double> ResultPercentageByCar4, QList<double> MaxPercentageByCar, int numDisplayCars)
{
    QT_CHARTS_USE_NAMESPACE
    //QList<QBarSet*> QBarList;
    QBarSeries *series = new QBarSeries();

    QBarSet *setResult1 = new QBarSet("Result Run 1");
    double average = 0;
    for(int i=0; i<ResultPercentageByCar1.size(); i++)
    {
        average += ResultPercentageByCar1.at(i);
        if(i<numDisplayCars)
        {
        *setResult1 << ResultPercentageByCar1.at(i);
        }
    }
    average = average / ResultPercentageByCar1.size();
    *setResult1 << average;
    series->append(setResult1);

    QBarSet *setResult2 = new QBarSet("Result Run 2");
    average = 0;
    for(int i=0; i<ResultPercentageByCar2.size(); i++)
    {
        average += ResultPercentageByCar2.at(i);
        if(i<numDisplayCars)
        {
        *setResult2 << ResultPercentageByCar2.at(i);
        }
    }
    average = average / ResultPercentageByCar2.size();
    *setResult2 << average;
    series->append(setResult2);

    QBarSet *setResult3 = new QBarSet("Result Run 3");
    average = 0;
    for(int i=0; i<ResultPercentageByCar3.size(); i++)
    {
        average += ResultPercentageByCar3.at(i);
        if(i<numDisplayCars)
        {
        *setResult3 << ResultPercentageByCar3.at(i);
        }
    }
    average = average / ResultPercentageByCar3.size();
    *setResult3 << average;
    series->append(setResult3);

    QBarSet *setResult4 = new QBarSet("Result Run 4");
    average = 0;
    for(int i=0; i<ResultPercentageByCar4.size(); i++)
    {
        average += ResultPercentageByCar4.at(i);
        if(i<numDisplayCars)
        {
        *setResult4 << ResultPercentageByCar4.at(i);
        }
    }
    average = average / ResultPercentageByCar4.size();
    *setResult4 << average;
    series->append(setResult4);


    QBarSet *setResult5 = new QBarSet("Result Max");
    average = 0;
    for(int i=0; i<MaxPercentageByCar.size(); i++)
    {
        average += MaxPercentageByCar.at(i);
        if(i<numDisplayCars)
        {
        *setResult5 << MaxPercentageByCar.at(i);
        }
    }
    average = average / MaxPercentageByCar.size();
    *setResult5 << average;
    series->append(setResult5);



    QStringList categories;
    for(int i=0; i<MaxPercentageByCar.size(); i++)
    {
        QString name ="Car ";
        name.append(QString::number(i));
        if(i<numDisplayCars)
        {
        categories << name;
        }
    }
    QString name ="OverAll ";
    name.append(QString::number(MaxPercentageByCar.size()));
    categories << name;

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

