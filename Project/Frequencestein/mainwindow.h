#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void setupGraph();
    void setupTimer();
private:
    Ui::MainWindow *ui;
    QTimer *timer;
public slots:
    void realtimeDataSlot();
};

#endif // MAINWINDOW_H
