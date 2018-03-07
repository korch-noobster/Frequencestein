#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(updateAudioLevelLabel()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_released()
{
    static bool isActive = false;
    if(!isActive)
    {
        dataTimer.start(0); // Interval 0 means to refresh as fast as possible
        audioInterface.start();
        isActive = true;
    }
    else
    {
        dataTimer.stop();
        audioInterface.stop();
        ui->label1->setText("Input off");
        isActive = false;
    }
}

void MainWindow::updateAudioLevelLabel()
{
    ui->label1->setText(QString::number(audioInterface.getValue()));
}
