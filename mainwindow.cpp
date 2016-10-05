#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

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
    if(dataPro.isAmmeterError())ammeterError();
    if(dataPro.isRegulatorError())regulatorError();
    connect(testWin,&TestWindow::testButtonClicked,&dataPro,&DataProcessor::testAction);
    connect(&dataPro,&DataProcessor::sendTestResult,testWin,&TestWindow::getResult);
    connect(&dataPro,&DataProcessor::monitorBegun,this,&MainWindow::disableTestWindow);
    connect(&dataPro,&DataProcessor::monitorFinish,this,&MainWindow::endableTestWindow);
    connect(&dataPro,&DataProcessor::ammeterError,this,&MainWindow::ammeterError);
    connect(&dataPro,&DataProcessor::regulatorError,this,&MainWindow::regulatorError);
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

void MainWindow::ammeterError()
{
    qDebug()<<"ammeter error";
    QMessageBox ammetermb;
    ammetermb.information(this,"电表错误","电表错误，请仔细检查电表连接，排除故障后再重启",QMessageBox::Ok);
        ui->historyButton->setEnabled(false);
        ui->realtimeButton->setEnabled(false);
        ui->testButton->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
        qApp->exit();
}

void MainWindow::regulatorError()
{
    qDebug()<<"regulator error";
    QMessageBox ammetermb;
    ammetermb.information(this,"下位机错误","下位机错误，请仔细检查下位机连接，排除故障后再重启",QMessageBox::Ok);
        //ui->historyButton->setEnabled(false);
        //ui->realtimeButton->setEnabled(false);
        ui->testButton->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
        qApp->exit();
}

void MainWindow::disableTestWindow()
{
    ui->testButton->setEnabled(false);

}

void MainWindow::endableTestWindow()
{
    ui->testButton->setEnabled(true);
}
