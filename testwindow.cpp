#include "testwindow.h"
#include "ui_testwindow.h"
#include "time.h".

/* TestWindow
 * Author: Wu Yuanhao
 */

TestWindow::TestWindow(QWidget *parent, DataProcessor *processor) :
    QWidget(parent),dataPro(processor),
    ui(new Ui::TestWindow)
{
    ui->setupUi(this);
    testModel.setHorizontalHeaderItem(0, new QStandardItem(QString("动作前功率/KW")));
    testModel.setHorizontalHeaderItem(1, new QStandardItem(QString("动作后功率/KW")));
    testModel.setHorizontalHeaderItem(2, new QStandardItem(QString("节电比率")));
    ui->testTable->setModel(&testModel);
    qsrand(time(NULL));
}

TestWindow::~TestWindow()
{
    delete ui;
}

void TestWindow::testEnd()
{
    TestRecord *latestRecord=&testRecords.last();

    //these twoline should be replaced by reading power data
    //from dataprocessor
    latestRecord->ratio=qrand()/(RAND_MAX+0.1);
    latestRecord->after=latestRecord->before*(1-latestRecord->ratio);

    int l=testRecords.length();
    testModel.setItem(l-1,0,new QStandardItem(QString("%1").arg(latestRecord->before)));
    testModel.setItem(l-1,1,new QStandardItem(QString("%1").arg(latestRecord->after)));
    testModel.setItem(l-1,2,new QStandardItem(QString("%1").arg(latestRecord->ratio)));

    float sum=0;
    for (int i=0;i<testRecords.length();i++)
    {
        sum+=testRecords[i].ratio;
    }
//    ui->avgNoLabel->setText(QString("%1").arg(sum/l));
}

void TestWindow::on_beginTest_clicked()
{
    TestRecord *newRecord=new TestRecord();

    //this line should be replaced by reading power data
    //from dataprocessor
//    newRecord->before=qrand();

//    testRecords.append(*newRecord);
    emit testButtonClicked();
}

void TestWindow::on_testEnd_clicked()
{
    this->testEnd();
}

void TestWindow::on_clearTestRecords_clicked()
{
    testRecords.clear();
    testModel.clear();
//    ui->avgNoLabel->setText(QString("0"));
}

void TestWindow::on_returnBtn_clicked()
{
    this->hide();
    dataPro->openMonitor();
}

void TestWindow::getResult(datatype powerBefore, datatype powerAfter, float ratio)
{
    int l=testRecords.length();
    testModel.setItem(l-1,0,new QStandardItem(QString("%1").arg(powerBefore)));
    testModel.setItem(l-1,1,new QStandardItem(QString("%1").arg(powerAfter)));
    testModel.setItem(l-1,2,new QStandardItem(QString("%1").arg(ratio)));
    TestRecord result;
    result.before=powerBefore;
    result.after=powerAfter;
    result.ratio=ratio;
    testRecords.append(result);
}

void TestWindow::on_calculateRatioButton_clicked()
{
    if(testRecords.size()<1)
    {
        QMessageBox::warning(this,QString("没有测试结果！"),QString("没有节电测试结果，无法计算平均节电率！"));
    }
    else
    {
        float sum=0;
        int len=testRecords.size();
        for (int i=0;i<len;i++)
        {
            sum+=testRecords[i].ratio;
        }
        ui->calculateRatioButton->setText(QString("%1").arg(sum/len));
    }
}
