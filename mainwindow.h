#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <realtimedatawindow.h>
#include <historydatawindow.h>
#include <configwindow.h>
#include <testwindow.h>
#include <datapoint.h>
#include <dataprocessor.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    RealTimeDataWindow realTimeWin;
    HistoryDataWindow historyWin;
    ConfigWindow configWin;
    TestWindow testWin;
    DataProcessor dataPro;




};

#endif // MAINWINDOW_H
