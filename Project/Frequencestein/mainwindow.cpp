#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>

#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    isSoundActive(false),
    isFrequencyActive(false)
{
    ui->setupUi(this);
    setupGraph();
    setupTimer();

    QTime currentTime(QTime::currentTime());
    timeForPlot = currentTime;

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
    //sound volume graphic
    ui->graphic1->addGraph(); // blue line
    ui->graphic1->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->graphic1->axisRect()->setupFullAxesBox();
    ui->graphic1->yAxis->setRange(-100, 100);

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
    ui->graphic2->xAxis->setRange(0, 16);
    ui->graphic2->yAxis->setRange(0, 20);
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
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->graphic1->xAxis->setRange(x, 8, Qt::AlignRight);
    ui->graphic1->graph(0)->rescaleValueAxis();
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

QVector <std::complex<double>> CombineResults(int n, QVector <std::complex<double>> b, QVector <std::complex<double>> c)
{
    QVector <std::complex<double>> image(n);
    for (int  i = 0; i < n / 2; i++)
    {
        image[2 * i] = b[i];
        image[2 * i + 1] = c[i];
    }
    return image;
}

QVector <std::complex<double>> BPF(QVector <std::complex<double>> a, bool minus = false){
    int n = a.size();
    if (n == 1) return a;

    QVector <std::complex<double>> c(n/2), b(n/2);
    std::complex<double> w = {1, 0};
    for(int j = 0; j < n/2; j++){
        b[j] = a[j] + a[j + n / 2];
        c[j] = (a[j] - a[j + n / 2]) * w;
        w *= std::complex<double> {cos(minus?2*M_PI/n:-2*M_PI/n), sin(minus?2*M_PI/n:-2*M_PI/n)};
    }
    return CombineResults(n, BPF(b, minus), BPF(c, minus));
}

void BPFdrow(QVector<double> &y, int N)
{
    QVector <double> X_f(N);
    QVector <double> Y_f(N);
    QVector <std::complex<double>> F(N);

    for(int i = 0; i < N; i++)
    {
        F[i] = std::complex<double>{y[i], 0};//myEquationComplex(i*2*M_PI/(N-1));
    }
    F = BPF(F);
    for(int i = 0; i < N; i++){
        X_f[i] = i;
        F[i] = std::complex<double> {F[i].real()/N, F[i].imag()/N};
        Y_f[i] = sqrt(pow(F[i].real(), 2) + pow(F[i].imag(), 2));
    }
}


void MainWindow::realtimeDataSlot()
{
    double key = timeForPlot.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    int N = 16;
    static int elementCount = 0;

    //data that will be ploted
    if (key - lastPointKey <= 0.001) // at most add point every 2 ms
        return;
    double data = 500*log(audioInterface.getValue()+1);

    if(isSoundActive)
        releaseSoundDiagram(key, data);

    static QVector<double> X(N), Y(N);
    if(elementCount < N)
    {
        X[elementCount] = elementCount + 1;
        Y[elementCount] = data;
        elementCount++;
    }
    if(elementCount == N-1)
    {
        BPFdrow(Y, N);
        if(isFrequencyActive)
            releaseFrequencyBars(X, Y);
        elementCount = 0;
    }

    ui->soundInfoLabel->setText(QString::number(data)+"dB");
    lastPointKey = key;
}

void MainWindow::on_startButton1_clicked()
{
    //static bool isActive = false;
    if(!isSoundActive)
    {
        timeForPlot.restart();
        timer->start(0); // Interval 0 means to refresh as fast as possible
        audioInterface.start();
        ui->startButton1->setText("STOP");
        isSoundActive = true;
    }
    else
    {
        //timer->stop();
        audioInterface.stop();
        ui->soundInfoLabel->setText("Press START button");
        ui->startButton1->setText("START");
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
