#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    realTimeWin=new RealTimeDataWindow(0,&dataPro);
    historyWin=new HistoryDataWindow(0,&dataPro);
    testWin=new TestWindow(0,&dataPro);
    realTimeWin->hide();
    historyWin->hide();
    testWin->hide();
    connect(testWin,&TestWindow::testButtonClicked,&dataPro,&DataProcessor::testAction);
    connect(&dataPro,&DataProcessor::sendTestResult,testWin,&TestWindow::getResult);
    connect(&dataPro,&DataProcessor::monitorBegun,this,&MainWindow::disableTestWindow);
    connect(&dataPro,&DataProcessor::monitorFinish,this,&MainWindow::endableTestWindow);
 }

MainWindow::~MainWindow()
{
    delete ui;
    delete realTimeWin;
    delete historyWin;
    delete testWin;
}

void MainWindow::on_realtimeButton_clicked()
{
    realTimeWin->show();
//    this->hide();
}

void MainWindow::on_testButton_clicked()
{
    testWin->show();
    dataPro.closeMonitor();
//    emit openTestWindow();
//    this->hide();
}

void MainWindow::on_historyButton_clicked()
{
    historyWin->show();
    //    this->hide();
}

void MainWindow::disableTestWindow()
{
    ui->testButton->setEnabled(false);

}

void MainWindow::endableTestWindow()
{
    ui->testButton->setEnabled(true);
}
