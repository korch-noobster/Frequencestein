#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
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
    ui->graphic->addGraph(); // blue line
    ui->graphic->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->graphic->axisRect()->setupFullAxesBox();
    ui->graphic->yAxis->setRange(-1.2, 1.2);
}

void MainWindow::setupTimer()
{
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    timer->start(10);   //time in milliseconds
}

void MainWindow::realtimeDataSlot()
{
    static QTime time(QTime::currentTime());
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;

    ///data that will be ploted
    double data = qSin(key)+qrand()/(double)RAND_MAX*1*qSin(key/0.3843);

    if (key - lastPointKey > 0.002) // at most add point every 2 ms
    {
      ui->graphic->graph(0)->addData(key, data);
      // rescale value (vertical) axis to fit the current data:
      ui->graphic->graph(0)->rescaleValueAxis();
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->graphic->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->graphic->replot();
}
