#include "testwindow.h"
#include "ui_testwindow.h"

TestWindow::TestWindow(QWidget *parent, DataProcessor *processor) :
    QWidget(parent),dataPro(processor),
    ui(new Ui::TestWindow)
{
    ui->setupUi(this);
    testModel.setHorizontalHeaderItem(0, new QStandardItem(QString("动作前功率/KW")));
    testModel.setHorizontalHeaderItem(1, new QStandardItem(QString("动作后功率/KW")));
    testModel.setHorizontalHeaderItem(2, new QStandardItem(QString("节电比率")));
    ui->testTable->setModel(&testModel);
}

void TestWindow::closeEvent(QCloseEvent *event)
{
    dataPro->openMonitor();
    qDebug()<<"test window closed";
    event->accept();
}

TestWindow::~TestWindow()
{
    delete ui;
}

void TestWindow::on_beginTest_clicked()
{
    emit testButtonClicked();
    qDebug()<<"test button clicked!";
}

void TestWindow::on_clearTestRecords_clicked()
{
    testRecords.clear();
    testModel.clear();
    testModel.setHorizontalHeaderItem(0, new QStandardItem(QString("动作前功率/KW")));
    testModel.setHorizontalHeaderItem(1, new QStandardItem(QString("动作后功率/KW")));
    testModel.setHorizontalHeaderItem(2, new QStandardItem(QString("节电比率")));
}

void TestWindow::on_returnBtn_clicked()
{
    this->hide();
    dataPro->openMonitor();
}

void TestWindow::getResult(datatype powerBefore, datatype powerAfter, float ratio)
{
    int l=testRecords.length();
    qDebug()<<"the lenght of testRecords is "<<l;
    testModel.setItem(l,0,new QStandardItem(QString("%1").arg(powerBefore,0,'f',3)));
    testModel.setItem(l,1,new QStandardItem(QString("%1").arg(powerAfter,0,'f',3)));
    testModel.setItem(l,2,new QStandardItem(QString("%1%").arg(ratio*100,0,'f',1)));
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
        ui->averageRatioLabel->setText(QString("%1%").arg(100*sum/len,0,'f',1));
    }
}
