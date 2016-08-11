#include "realtimedatawindow.h"
#include "ui_realtimedatawindow.h"

RealTimeDataWindow::RealTimeDataWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RealTimeDataWindow)
{
    ui->setupUi(this);
    range=60;
    mode=VMODE;
    //connect(updateTimer,SIGNAL(timeout()),this,SLOT(addDataToPlot()));
    //updateTimer.start(1000);
    replot(mode,range);
}

RealTimeDataWindow::~RealTimeDataWindow()
{
    delete ui;
}

void RealTimeDataWindow::on_vRadioBtn_toggled(bool checked)
{
    if (checked)
        if (mode!=VMODE)
        {
            replot(VMODE,range);
            mode=VMODE;
        }
}

void RealTimeDataWindow::on_iRadioBtn_toggled(bool checked)
{
    if (checked)
        if (mode!=IMODE)
        {
            replot(IMODE,range);
            mode=IMODE;
        }
}

void RealTimeDataWindow::on_epRadioBtn_toggled(bool checked)
{
    if (checked)
        if (mode!=EPMODE)
        {
            replot(EPMODE,range);
            mode=EPMODE;
        }
}

void RealTimeDataWindow::on_fpRadioBtn_toggled(bool checked)
{
    if (checked)
        if (mode!=RPMODE)
        {
            replot(RPMODE,range);
            mode=RPMODE;
        }
}

void RealTimeDataWindow::on_apRadioBtn_toggled(bool checked)
{
    if (checked)
        if (mode!=APMODE)
        {
            replot(APMODE,range);
            mode=APMODE;
        }
}

void RealTimeDataWindow::on_pfRadioBtn_toggled(bool checked)
{
    if (checked)
        if (mode!=PFMODE)
        {
            replot(PFMODE,range);
            mode=PFMODE;
        }
}

void RealTimeDataWindow::trimdata(int range)
{
    while (datapoints.length()>range)
        datapoints.pop_front();
}


void RealTimeDataWindow::replot(int mode, int range)
{
    ui->plot->clearGraphs();
    ui->plot->addGraph();
    ui->plot->addGraph();
    ui->plot->addGraph();

    ui->plot->graph(0)->setName(QString("a"));
    ui->plot->graph(1)->setName(QString("b"));
    ui->plot->graph(2)->setName(QString("c"));

    ui->plot->graph(0)->setPen(QPen(Qt::red));
    ui->plot->graph(1)->setPen(QPen(Qt::blue));
    ui->plot->graph(2)->setPen(QPen(Qt::green));

    if (mode!=VMODE && mode!=IMODE)
    {
        ui->plot->addGraph();
        ui->plot->graph(3)->setPen(QPen(Qt::black));
        ui->plot->graph(3)->setName(QString("s"));
    }

    ui->plot->legend->setVisible(true);
    ui->plot->xAxis->setRange(0,range);
    ui->plot->rescaleAxes(true);

    removeIdx=0;
    int i;
    for (i=0;i<range;i++)
    {
        if (i>=datapoints.length())
            break;
        if (mode==VMODE)
        {
            ui->plot->graph(0)->addData(i,datapoints[i].va);
            ui->plot->graph(1)->addData(i,datapoints[i].vb);
            ui->plot->graph(2)->addData(i,datapoints[i].vc);
        }
        else if (mode==IMODE)
        {
            ui->plot->graph(0)->addData(i,datapoints[i].ia);
            ui->plot->graph(1)->addData(i,datapoints[i].ib);
            ui->plot->graph(2)->addData(i,datapoints[i].ic);
        }
        else if (mode==EPMODE)
        {
            ui->plot->graph(0)->addData(i,datapoints[i].epa);
            ui->plot->graph(1)->addData(i,datapoints[i].epb);
            ui->plot->graph(2)->addData(i,datapoints[i].epc);
            ui->plot->graph(3)->addData(i,datapoints[i].eps);
        }
        else if (mode==RPMODE)
        {
            ui->plot->graph(0)->addData(i,datapoints[i].rpa);
            ui->plot->graph(1)->addData(i,datapoints[i].rpb);
            ui->plot->graph(2)->addData(i,datapoints[i].rpc);
            ui->plot->graph(3)->addData(i,datapoints[i].rps);
        }
        else if (mode==APMODE)
        {
            ui->plot->graph(0)->addData(i,datapoints[i].apa);
            ui->plot->graph(1)->addData(i,datapoints[i].apb);
            ui->plot->graph(2)->addData(i,datapoints[i].apc);
            ui->plot->graph(3)->addData(i,datapoints[i].aps);
        }
        else if (mode==PFMODE)
        {
            ui->plot->graph(0)->addData(i,datapoints[i].pfa);
            ui->plot->graph(1)->addData(i,datapoints[i].pfb);
            ui->plot->graph(2)->addData(i,datapoints[i].pfc);
            ui->plot->graph(3)->addData(i,datapoints[i].pfs);
        }
    }
    addIdx=i;
    ui->plot->replot();
}

void RealTimeDataWindow::on_horizontalSlider_sliderReleased()
{
    qDebug()<<ui->horizontalSlider->value();
    int newrange=ui->horizontalSlider->value();
    if (newrange==0)
        newrange=60;
    else if (newrange==1)
        newrange=600;
    else
        newrange=3600;
    if (newrange!=range)
    {
        range=newrange;
        trimdata(range);
        replot(mode,range);
    }
}

void RealTimeDataWindow::addDataToPlot()
{
    DataPoint newdata;  //read data from dataprocess

    datapoints.append(newdata);
    if (datapoints.length()>range)
    {
        datapoints.pop_front();
        ui->plot->graph(0)->data()->remove(removeIdx);
        ui->plot->graph(1)->data()->remove(removeIdx);
        ui->plot->graph(2)->data()->remove(removeIdx);
        if (mode!=VMODE && mode!=IMODE)
            ui->plot->graph(3)->data()->remove(removeIdx);
        removeIdx++;
    }
    addIdx++;
    if (mode==VMODE)
    {
        ui->plot->graph(0)->addData(addIdx,newdata.va);
        ui->plot->graph(1)->addData(addIdx,newdata.vb);
        ui->plot->graph(2)->addData(addIdx,newdata.vc);
    }
    else if (mode==IMODE)
    {
        ui->plot->graph(0)->addData(addIdx,newdata.ia);
        ui->plot->graph(1)->addData(addIdx,newdata.ib);
        ui->plot->graph(2)->addData(addIdx,newdata.ic);
    }
    else if (mode==EPMODE)
    {
        ui->plot->graph(0)->addData(addIdx,newdata.epa);
        ui->plot->graph(1)->addData(addIdx,newdata.epb);
        ui->plot->graph(2)->addData(addIdx,newdata.epc);
        ui->plot->graph(3)->addData(addIdx,newdata.eps);
    }
    else if (mode==RPMODE)
    {
        ui->plot->graph(0)->addData(addIdx,newdata.rpa);
        ui->plot->graph(1)->addData(addIdx,newdata.rpb);
        ui->plot->graph(2)->addData(addIdx,newdata.rpc);
        ui->plot->graph(3)->addData(addIdx,newdata.rps);
    }
    else if (mode==APMODE)
    {
        ui->plot->graph(0)->addData(addIdx,newdata.apa);
        ui->plot->graph(1)->addData(addIdx,newdata.apb);
        ui->plot->graph(2)->addData(addIdx,newdata.apc);
        ui->plot->graph(3)->addData(addIdx,newdata.aps);
    }
    else if (mode==PFMODE)
    {
        ui->plot->graph(0)->addData(addIdx,newdata.pfa);
        ui->plot->graph(1)->addData(addIdx,newdata.pfb);
        ui->plot->graph(2)->addData(addIdx,newdata.pfc);
        ui->plot->graph(3)->addData(addIdx,newdata.pfs);
    }

}
