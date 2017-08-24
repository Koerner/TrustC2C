#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mapmodel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

 }

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setProgressBar(int i)
{
    this->ui->progressBar->setValue(i);
}

void MainWindow::setProgressBarCar1(double i)
{
    this->ui->progressBar_2->setValue(i*100);
}

void MainWindow::setProgressBarCar2(double i)
{
    this->ui->progressBar_3->setValue(i*100);
}
