#include "historydatawindow.h"
#include "ui_historydatawindow.h"

HistoryDataWindow::HistoryDataWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryDataWindow)
{
    ui->setupUi(this);
    range=3600;
    mode=VMODE;
    const char plotnames[4]={'a','b','c','s'};
    const int plotcolors[4]={Qt::red,Qt::blue,Qt::black,Qt::green};
    for (int i=0;i<4;i++)
    {
        plots[i].addGraph();
        ui->plotLayout->addWidget((QWidget*)&plots[i]);
        plots[i].graph(0)->setName(QString(plotnames[i]));
        plots[i].legend->setVisible(true);
        plots[i].graph(0)->setPen(QPen(plotcolors[i]));
    }
    replot(mode,range);
}

HistoryDataWindow::~HistoryDataWindow()
{
    delete ui;
}

void HistoryDataWindow::on_vRadioBtn_toggled(bool checked)
{
    if (checked)
        if (mode!=VMODE)
        {
            replot(VMODE,range);
            mode=VMODE;
        }
}

void HistoryDataWindow::on_iRadioBtn_toggled(bool checked)
{
    if (checked)
        if (mode!=IMODE)
        {
            replot(IMODE,range);
            mode=IMODE;
        }
}

void HistoryDataWindow::on_epRadioBtn_toggled(bool checked)
{
    if (checked)
        if (mode!=EPMODE)
        {
            replot(EPMODE,range);
            mode=EPMODE;
        }
}

void HistoryDataWindow::on_fpRadioBtn_toggled(bool checked)
{
    if (checked)
        if (mode!=RPMODE)
        {
            replot(RPMODE,range);
            mode=RPMODE;
        }
}

void HistoryDataWindow::on_apRadioBtn_toggled(bool checked)
{
    if (checked)
        if (mode!=APMODE)
        {
            replot(APMODE,range);
            mode=APMODE;
        }
}

void HistoryDataWindow::on_pfRadioBtn_toggled(bool checked)
{
    if (checked)
        if (mode!=PFMODE)
        {
            replot(PFMODE,range);
            mode=PFMODE;
        }
}


void HistoryDataWindow::replot(int mode,int range)
{
    for (int i=0;i<3;i++)
    {
        plots[i].graph(0)->clearData();
        plots[i].xAxis->setRange(0,range);
    }

    if (mode==VMODE || mode==IMODE)
    {
        ui->plotLayout->removeWidget(&plots[3]);
        plots[3].hide();
    }
    else
    {
        if (ui->plotLayout->count()==3)
        {
            ui->plotLayout->addWidget(&plots[3]);
            plots[3].show();
        }
        plots[3].graph(0)->clearData();
        plots[3].xAxis->setRange(0,range);
    }

    int i;
    for (i=0;i<range;i++)
    {
        if (i>=datapoints.length())
            break;
        if (mode==VMODE)
        {
            plots[0].graph(0)->addData(i,datapoints[i].va);
            plots[1].graph(0)->addData(i,datapoints[i].vb);
            plots[2].graph(0)->addData(i,datapoints[i].vc);
        }
        else if (mode==IMODE)
        {
            plots[0].graph(0)->addData(i,datapoints[i].ia);
            plots[1].graph(0)->addData(i,datapoints[i].ib);
            plots[2].graph(0)->addData(i,datapoints[i].ic);
        }
        else if (mode==EPMODE)
        {
            plots[0].graph(0)->addData(i,datapoints[i].epa);
            plots[1].graph(0)->addData(i,datapoints[i].epb);
            plots[2].graph(0)->addData(i,datapoints[i].epc);
            plots[3].graph(0)->addData(i,datapoints[i].eps);
        }
        else if (mode==RPMODE)
        {
            plots[0].graph(0)->addData(i,datapoints[i].rpa);
            plots[1].graph(0)->addData(i,datapoints[i].rpb);
            plots[2].graph(0)->addData(i,datapoints[i].rpc);
            plots[3].graph(0)->addData(i,datapoints[i].rps);
        }
        else if (mode==APMODE)
        {
            plots[0].graph(0)->addData(i,datapoints[i].apa);
            plots[1].graph(0)->addData(i,datapoints[i].apb);
            plots[2].graph(0)->addData(i,datapoints[i].apc);
            plots[3].graph(0)->addData(i,datapoints[i].aps);
        }
        else if (mode==PFMODE)
        {
            plots[0].graph(0)->addData(i,datapoints[i].pfa);
            plots[1].graph(0)->addData(i,datapoints[i].pfb);
            plots[2].graph(0)->addData(i,datapoints[i].pfc);
            plots[3].graph(0)->addData(i,datapoints[i].pfs);
        }
    }
    for (int i=0;i<4;i++)
        plots[i].replot();
}

void HistoryDataWindow::on_setRangeBtn_clicked()
{
    QDateTime beginDateTime,endDateTime;
    beginDateTime.setDate(ui->beginDate->date());
    beginDateTime.setTime(QTime(ui->beginHour->value(),0,0,0));

    endDateTime.setDate(ui->endDate->date());
    endDateTime.setTime(QTime(ui->endHour->value(),0,0,0));

//    dataPro->dataSlicer(beginDateTime,endDateTime,360,datapoints);
    range=360;
    replot(mode,range);
}

void HistoryDataWindow::on_returnBtn_clicked()
{
    this->hide();
    parentWin->show();
}

void HistoryDataWindow::setParentWin(QWidget* p)
{
    parentWin=p;
}

