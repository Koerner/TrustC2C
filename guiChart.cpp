#include "guiChart.h"
#include "guitableforchart.h"

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QTableView>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QVXYModelMapper>
#include <QtWidgets/QHeaderView>
#include <QScatterSeries>


QT_CHARTS_USE_NAMESPACE

guiChart::guiChart(logDatabase &logDatabaseHandover, database &databaseHandover, int CarID, int first, int last, QWidget *parent)
    : QWidget(parent)
{
    // create simple model for storing data
    // user's table data model
    //! [1]
    guiTableForChart *model = new guiTableForChart(logDatabaseHandover,databaseHandover, CarID, first, last);
    //! [1]

    //! [2]
    // create table view and add model to it
    QTableView *tableView = new QTableView;
    tableView->setModel(model);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //! [2]

    //
    // First chart
    //

    //! [3]
    QChart *chart = new QChart;
    chart->setAnimationOptions(QChart::AllAnimations);
    QVXYModelMapper *mapper = new QVXYModelMapper(this);
    // for storing color hex from the series
    QString seriesColorHex = "#000000";


    // series 2-5
    for(int i=2; i<3+1; i++)
    {
        //! [6]

        mapper = new QVXYModelMapper(this);
        mapper->setXColumn(1);
        mapper->setYColumn(i);

        if(i==2)
        {
            QScatterSeries *series2 = new QScatterSeries;
            QString str = "Column ";
            str.append(QString::number(i+1));
            series2->setName(str);

            mapper->setSeries(series2);
            mapper->setModel(model);
            series2->setMarkerSize(7);
            chart->addSeries(series2);


            // get the color of the series and use it for showing the mapped area
            seriesColorHex = "#" + QString::number(series2->pen().color().rgb(), 16).right(6).toUpper();
            model->addMapping(seriesColorHex, QRect(i, 0, 1, model->rowCount()));
        }
        else
        {
            QLineSeries *series2 = new QLineSeries;
            QString str = "Column ";
            str.append(QString::number(i+1));
            series2->setName(str);

            mapper->setSeries(series2);
            mapper->setModel(model);
            chart->addSeries(series2);

            // get the color of the series and use it for showing the mapped area
            seriesColorHex = "#" + QString::number(series2->pen().color().rgb(), 16).right(6).toUpper();
            model->addMapping(seriesColorHex, QRect(i, 0, 1, model->rowCount()));
        }

    }

    //
    //second chart
    //

    QChart *chart2 = new QChart;
    chart2->setAnimationOptions(QChart::AllAnimations);


    // series 2-5
    for(int i=4; i<=8; i++)
    {
        //! [6]

        mapper = new QVXYModelMapper(this);
        mapper->setXColumn(1);
        mapper->setYColumn(i);

        if(i==4 || i==6 || i==8)
        {
            QScatterSeries *series2 = new QScatterSeries;
            QString str = "Column ";
            str.append(QString::number(i+1));
            series2->setName(str);

            mapper->setSeries(series2);
            mapper->setModel(model);
            series2->setMarkerSize(7);
            chart2->addSeries(series2);


            // get the color of the series and use it for showing the mapped area
            seriesColorHex = "#" + QString::number(series2->pen().color().rgb(), 16).right(6).toUpper();
            model->addMapping(seriesColorHex, QRect(i, 0, 1, model->rowCount()));
        }
        else
        {
            QLineSeries *series2 = new QLineSeries;
            QString str = "Column ";
            str.append(QString::number(i+1));
            series2->setName(str);

            mapper->setSeries(series2);
            mapper->setModel(model);

            chart2->addSeries(series2);

            // get the color of the series and use it for showing the mapped area
            seriesColorHex = "#" + QString::number(series2->pen().color().rgb(), 16).right(6).toUpper();
            model->addMapping(seriesColorHex, QRect(i, 0, 1, model->rowCount()));
        }

    }

    //! [8]
    chart->createDefaultAxes();
    chart->axisY()->setMax(1.01);
    chart->axisY()->setMin(-0.01);
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(500, 200);

    chart2->createDefaultAxes();
    chart2->axisY()->setMax(1.01);
    chart2->axisY()->setMin(-0.01);
    QChartView *chartView2 = new QChartView(chart2);
    chartView2->setRenderHint(QPainter::Antialiasing);
    chartView2->setMinimumSize(500, 200);
    //! [8]

    //! [9]
    // create main layout
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(tableView, 1, 0, 2, 1);
    mainLayout->setColumnStretch(0, 7);

    QGridLayout *chartLayout = new QGridLayout;
    mainLayout->addWidget(chartView, 1, 1);
    mainLayout->addWidget(chartView2, 2, 1);
    mainLayout->setColumnStretch(1, 9);

    setLayout(mainLayout);
    //! [9]
}

