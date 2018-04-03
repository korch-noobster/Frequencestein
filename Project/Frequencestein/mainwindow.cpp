#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    isSoundActive(false),
    isFrequencyActive(false)
{
    ui->setupUi(this);
    setupGraph();
    setupTimer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupGraph()
{
    //sound volume graphic
    ui->graphic1->addGraph(); // blue line
    ui->graphic1->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->graphic1->axisRect()->setupFullAxesBox();
    ui->graphic1->yAxis->setRange(-1.2, 1.2);

    //sound frequency bars
    ui->graphic2->removePlottable(0);
    frequencyBars = new QCPBars(ui->graphic2->xAxis, ui->graphic2->yAxis);
    //frequencyBars->setData(x,y);
    frequencyBars->setWidth(1);
    ui->graphic2->addPlottable(frequencyBars);
    QPen pen;
    pen.setWidthF(1);
    pen.setColor(QColor(Qt::black));
    frequencyBars->setPen(Qt::NoPen);
    frequencyBars->setBrush(QColor(205,100,0, 150));
    frequencyBars->setPen(pen);

    ui->graphic2->axisRect()->setupFullAxesBox();
    ui->graphic2->xAxis->setRange(0, 20);
    ui->graphic2->yAxis->setRange(0, 30);
//    ui->graphic2->replot();
}

void MainWindow::setupTimer()
{
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
}

void MainWindow::releaseSoundDiagram(double x, double y)
{
    ui->graphic1->graph(0)->addData(x, y);
    // rescale value (vertical) axis to fit the current data:
    ui->graphic1->graph(0)->rescaleValueAxis();
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->graphic1->xAxis->setRange(x, 8, Qt::AlignRight);
    ui->graphic1->replot();
}

void MainWindow::releaseFrequencyBars(QVector<double> x, QVector<double> y)
{

    //sound frequency bars
    ui->graphic2->removePlottable(0);
    frequencyBars = new QCPBars(ui->graphic2->xAxis, ui->graphic2->yAxis);
    frequencyBars->setData(x,y);
    frequencyBars->setWidth(1);
    ui->graphic2->addPlottable(frequencyBars);
    QPen pen;
    pen.setWidthF(1);
    pen.setColor(QColor(Qt::black));
    frequencyBars->setPen(Qt::NoPen);
    frequencyBars->setBrush(QColor(205,100,0, 150));
    frequencyBars->setPen(pen);

    ui->graphic2->axisRect()->setupFullAxesBox();
    ui->graphic2->replot();
}

void MainWindow::realtimeDataSlot()
{
    static QTime time(QTime::currentTime());
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;

    //data that will be ploted
    double data = audioInterface.getValue();

    if (key - lastPointKey <= 0.002) // at most add point every 2 ms
        return;
    if(isSoundActive)
        releaseSoundDiagram(key, data);

    QVector<double> X(20), Y(20);
    for (int i = 0; i < 20; ++i)
    {
        X[i] = i;
        Y[i] = rand()%20;
    }

    if (key - lastPointKey <= 0.01) // at most add point every 2 ms
        return;
    if(isFrequencyActive)
        releaseFrequencyBars(X, Y);
    lastPointKey = key;
}

void MainWindow::on_startButton1_clicked()
{
    //static bool isActive = false;
    if(!isSoundActive)
    {
        timer->start(0); // Interval 0 means to refresh as fast as possible
        audioInterface.start();
        ui->label1->setText("Input on");
        isSoundActive = true;
    }
    else
    {
        //timer->stop();
        audioInterface.stop();
        ui->label1->setText("Input off");
        isSoundActive = false;
    }
}

void MainWindow::on_startButton2_clicked()
{
    if(!isFrequencyActive)
    {
        timer->start(0); // Interval 0 means to refresh as fast as possible
        audioInterface.start();
        ui->label2->setText("Input on");
        isFrequencyActive = true;
    }
    else
    {
        ui->label2->setText("Input off");
        isFrequencyActive = false;
    }
}
