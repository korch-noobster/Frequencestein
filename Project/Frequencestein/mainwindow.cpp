#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupGraph();
    setupTimer();

    this->move(0,0);
    this->resize(QApplication::desktop()->availableGeometry().width(), QApplication::desktop()->availableGeometry().height());
    this->showMaximized();
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
}

void MainWindow::realtimeDataSlot()
{
    static QTime time(QTime::currentTime());
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    static double lastData = 0;
    ///data that will be ploted
    double data = 500*log(audioInterface.getValue()+1);
    //data = (data*0.8) + (lastData*0.2);
    //lastData = data;

    if (key - lastPointKey > 0.002) // at most add point every 2 ms
    {
      ui->graphic->graph(0)->addData(key, data);
      // rescale value (vertical) axis to fit the current data:
      ui->graphic->graph(0)->rescaleValueAxis();
      lastPointKey = key;
    }

    ui->soundInfoLabel->setText(QString::number(data)+"dB");
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->graphic->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->graphic->replot();
}

void MainWindow::on_startButton_released()
{
    static bool isActive = false;
    if(!isActive)
    {
        timer->start(0); // Interval 0 means to refresh as fast as possible
        audioInterface.start();
        isActive = true;
        ui->startButton->setText("STOP");
    }
    else
    {
        timer->stop();
        audioInterface.stop();
        ui->soundInfoLabel->setText("Press START button");
        ui->startButton->setText("START");
        isActive = false;
    }
}
