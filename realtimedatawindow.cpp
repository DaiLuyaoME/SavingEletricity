#include "realtimedatawindow.h"
#include "ui_realtimedatawindow.h"

RealTimeDataWindow::RealTimeDataWindow(QWidget *parent,DataProcessor *processor) :
    QWidget(parent),
    ui(new Ui::RealTimeDataWindow),dataPro(processor)
{
    ui->setupUi(this);
    range=60;
    mode=VMODE;
    connect(dataPro,&DataProcessor::newRealTimeData,this,&RealTimeDataWindow::addDataToPlot);
    const char plotnames[4]={'a','b','c','s'};
    const int plotcolors[4]={Qt::red,Qt::blue,Qt::black,Qt::green};
    for (int i=0;i<4;i++)
    {
        plots[i].addGraph();
        ui->playLayout->addWidget((QWidget*)&plots[i]);
        plots[i].graph(0)->setName(QString(plotnames[i]));
        plots[i].legend->setVisible(true);
        plots[i].graph(0)->setPen(QPen(plotcolors[i]));
    }
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
    for (int i=0;i<3;i++)
    {
        plots[i].graph(0)->clearData();
        plots[i].xAxis->setRange(0,range);
    }

    if (mode==VMODE || mode==IMODE)
    {
        ui->playLayout->removeWidget(&plots[3]);
        plots[3].hide();
    }
    else
    {
        if (ui->playLayout->count()==3)
        {
            ui->playLayout->addWidget(&plots[3]);
            plots[3].show();
        }
        plots[3].graph(0)->clearData();
        plots[3].xAxis->setRange(0,range);
    }

    removeIdx=0;
    int i;
    for (i=0;i<datapoints.length();i++)
    {
        if (i>=datapoints.length())
            break;
        if (mode==VMODE)
        {
            plots[0].graph(0)->addData(addIdx,datapoints[i].va);
            plots[1].graph(0)->addData(addIdx,datapoints[i].vb);
            plots[2].graph(0)->addData(addIdx,datapoints[i].vc);
        }
        else if (mode==IMODE)
        {
            plots[0].graph(0)->addData(addIdx,datapoints[i].ia);
            plots[1].graph(0)->addData(addIdx,datapoints[i].ib);
            plots[2].graph(0)->addData(addIdx,datapoints[i].ic);
        }
        else if (mode==EPMODE)
        {
            plots[0].graph(0)->addData(addIdx,datapoints[i].epa);
            plots[1].graph(0)->addData(addIdx,datapoints[i].epb);
            plots[2].graph(0)->addData(addIdx,datapoints[i].epc);
            plots[3].graph(0)->addData(addIdx,datapoints[i].eps);
        }
        else if (mode==RPMODE)
        {
            plots[0].graph(0)->addData(addIdx,datapoints[i].rpa);
            plots[1].graph(0)->addData(addIdx,datapoints[i].rpb);
            plots[2].graph(0)->addData(addIdx,datapoints[i].rpc);
            plots[3].graph(0)->addData(addIdx,datapoints[i].rps);
        }
        else if (mode==APMODE)
        {
            plots[0].graph(0)->addData(addIdx,datapoints[i].apa);
            plots[1].graph(0)->addData(addIdx,datapoints[i].apb);
            plots[2].graph(0)->addData(addIdx,datapoints[i].apc);
            plots[3].graph(0)->addData(addIdx,datapoints[i].aps);
        }
        else if (mode==PFMODE)
        {
            plots[0].graph(0)->addData(addIdx,datapoints[i].pfa);
            plots[1].graph(0)->addData(addIdx,datapoints[i].pfb);
            plots[2].graph(0)->addData(addIdx,datapoints[i].pfc);
            plots[3].graph(0)->addData(addIdx,datapoints[i].pfs);
        }
    }
    addIdx=i;
    for (int i=0;i<4;i++)
        plots[i].replot();
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
    newdata=dataPro->getLastData();
    qDebug()<<"window get data"<<newdata.apa<<endl;

    datapoints.append(newdata);
    if (datapoints.length()>range)
    {
        datapoints.pop_front();
        for (int i=0;i<3;i++)
            plots[i].graph(0)->data()->remove(removeIdx);
        if (mode!=VMODE && mode!=IMODE)
            plots[3].graph(0)->data()->remove(removeIdx);
        removeIdx++;
    }
    addIdx++;
    if (mode==VMODE)
    {
        plots[0].graph(0)->addData(addIdx,newdata.va);
        plots[1].graph(0)->addData(addIdx,newdata.vb);
        plots[2].graph(0)->addData(addIdx,newdata.vc);
    }
    else if (mode==IMODE)
    {
        plots[0].graph(0)->addData(addIdx,newdata.ia);
        plots[1].graph(0)->addData(addIdx,newdata.ib);
        plots[2].graph(0)->addData(addIdx,newdata.ic);
    }
    else if (mode==EPMODE)
    {
        plots[0].graph(0)->addData(addIdx,newdata.epa);
        plots[1].graph(0)->addData(addIdx,newdata.epb);
        plots[2].graph(0)->addData(addIdx,newdata.epc);
        plots[3].graph(0)->addData(addIdx,newdata.eps);
    }
    else if (mode==RPMODE)
    {
        plots[0].graph(0)->addData(addIdx,newdata.rpa);
        plots[1].graph(0)->addData(addIdx,newdata.rpb);
        plots[2].graph(0)->addData(addIdx,newdata.rpc);
        plots[3].graph(0)->addData(addIdx,newdata.rps);
    }
    else if (mode==APMODE)
    {
        plots[0].graph(0)->addData(addIdx,newdata.apa);
        plots[1].graph(0)->addData(addIdx,newdata.apb);
        plots[2].graph(0)->addData(addIdx,newdata.apc);
        plots[3].graph(0)->addData(addIdx,newdata.aps);
    }
    else if (mode==PFMODE)
    {
        plots[0].graph(0)->addData(addIdx,newdata.pfa);
        plots[1].graph(0)->addData(addIdx,newdata.pfb);
        plots[2].graph(0)->addData(addIdx,newdata.pfc);
        plots[3].graph(0)->addData(addIdx,newdata.pfs);
    }

}
