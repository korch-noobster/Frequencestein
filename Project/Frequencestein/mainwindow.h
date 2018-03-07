#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAudioRecorder>
#include <QTimer>
#include "audiointerface.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_startButton_released();
    void updateAudioLevelLabel();

private:
    Ui::MainWindow *ui;
    QTimer dataTimer;
    AudioInterface audioInterface;

};

#endif // MAINWINDOW_H
