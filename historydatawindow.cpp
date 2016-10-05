#include "historydatawindow.h"
#include "ui_historydatawindow.h"

HistoryDataWindow::HistoryDataWindow(QWidget *parent, DataProcessor *processor) :
    QWidget(parent),dataPro(processor),
    ui(new Ui::HistoryDataWindow)
{
    ui->setupUi(this);
    range=3600;
    mode=VMODE;
    //    mode=NONMODE;
    const char plotnames[4]={'a','b','c','s'};
    Qt::GlobalColor plotcolors[4]={Qt::red,Qt::blue,Qt::black,Qt::green};
    for (int i=0;i<4;i++)
    {
        plots[i].addGraph();
        ui->plotLayout->addWidget((QWidget*)&plots[i]);
        plots[i].graph(0)->setName(QString(plotnames[i]));
        plots[i].legend->setVisible(true);
        plots[i].graph(0)->setPen(QPen(plotcolors[i]));
    }
    replot(mode,range);
    ui->endDate->setDate(QDate::currentDate());
    ui->beginDate->setDate(QDate::currentDate());
    ui->endHour->setValue(QTime::currentTime().hour());
    ui->beginHour->setValue(QTime::currentTime().hour()-1);
}

HistoryDataWindow::~HistoryDataWindow()
{
    delete ui;
}

void HistoryDataWindow::on_vRadioBtn_toggled(bool checked)
{
    if (checked)
    {
//        if (mode!=VMODE)
//        {
            replot(VMODE,range);
            mode=VMODE;
            for (int i=0;i<3;++i)
            {
                plots[i].yAxis->setLabel("电压/V");
            }
//        }

    }
}

void HistoryDataWindow::on_iRadioBtn_toggled(bool checked)
{
    if (checked)
    {
//        if (mode!=IMODE)
//        {
            replot(IMODE,range);
            mode=IMODE;
            for (int i=0;i<3;++i)
            {
                plots[i].yAxis->setLabel("电流/A");
            }
//        }

    }
}

void HistoryDataWindow::on_epRadioBtn_toggled(bool checked)
{
    if (checked)
    {
        if (mode!=EPMODE)
        {
            replot(EPMODE,range);
            mode=EPMODE;
            for (int i=0;i<4;++i)
            {
                plots[i].yAxis->setLabel("有功功率/KW");
            }
        }

    }
}

void HistoryDataWindow::on_fpRadioBtn_toggled(bool checked)
{
    if (checked)
    {
        if (mode!=RPMODE)
        {
            replot(RPMODE,range);
            mode=RPMODE;
            for (int i=0;i<4;++i)
            {
                plots[i].yAxis->setLabel("无功功率/Var");
            }
        }

    }
}

void HistoryDataWindow::on_apRadioBtn_toggled(bool checked)
{
    if (checked)
    {
        if (mode!=APMODE)
        {
            replot(APMODE,range);
            mode=APMODE;
            for (int i=0;i<4;++i)
            {
                plots[i].yAxis->setLabel("视在功率/KVA");
            }
        }

    }
}

void HistoryDataWindow::on_pfRadioBtn_toggled(bool checked)
{
    if (checked)
    {
        if (mode!=PFMODE)
        {
            replot(PFMODE,range);
            mode=PFMODE;
            for (int i=0;i<4;++i)
            {
                plots[i].yAxis->setLabel("功率因素/KVA");
            }
        }

    }
}


void HistoryDataWindow::replot(int mode,int range)
{
    //qDebug()<<"call plot************************";
    //qDebug()<<"current mode is "<<mode;
    for (int i=0;i<4;i++)
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


    for (int i=0;i<range;i++)
    {
        if (i>=datapoints.length())
            break;
        if (mode==VMODE)
        {
            plots[0].graph(0)->addData(i,datapoints[i].va);
            plots[1].graph(0)->addData(i,datapoints[i].vb);
            plots[2].graph(0)->addData(i,datapoints[i].vc);
            for (int i=0;i<3;++i)
            {
                plots[i].yAxis->setLabel("电压/V");
            }
        }
        else if (mode==IMODE)
        {
            plots[0].graph(0)->addData(i,datapoints[i].ia);
            plots[1].graph(0)->addData(i,datapoints[i].ib);
            plots[2].graph(0)->addData(i,datapoints[i].ic);
            for (int i=0;i<3;++i)
            {
                plots[i].yAxis->setLabel("电流/A");
            }
        }
        else if (mode==EPMODE)
        {
            plots[0].graph(0)->addData(i,datapoints[i].epa);
            plots[1].graph(0)->addData(i,datapoints[i].epb);
            plots[2].graph(0)->addData(i,datapoints[i].epc);
            plots[3].graph(0)->addData(i,datapoints[i].eps);
            for (int i=0;i<4;++i)
            {
                plots[i].yAxis->setLabel("有功功率/KW");
            }
        }
        else if (mode==RPMODE)
        {
            plots[0].graph(0)->addData(i,datapoints[i].rpa);
            plots[1].graph(0)->addData(i,datapoints[i].rpb);
            plots[2].graph(0)->addData(i,datapoints[i].rpc);
            plots[3].graph(0)->addData(i,datapoints[i].rps);
            for (int i=0;i<4;++i)
            {
                plots[i].yAxis->setLabel("无功功率/Var");
            }
        }
        else if (mode==APMODE)
        {
            plots[0].graph(0)->addData(i,datapoints[i].apa);
            plots[1].graph(0)->addData(i,datapoints[i].apb);
            plots[2].graph(0)->addData(i,datapoints[i].apc);
            plots[3].graph(0)->addData(i,datapoints[i].aps);
            for (int i=0;i<4;++i)
            {
                plots[i].yAxis->setLabel("视在功率/KVA");
            }
        }
        else if (mode==PFMODE)
        {
            plots[0].graph(0)->addData(i,datapoints[i].pfa);
            plots[1].graph(0)->addData(i,datapoints[i].pfb);
            plots[2].graph(0)->addData(i,datapoints[i].pfc);
            plots[3].graph(0)->addData(i,datapoints[i].pfs);
            for (int i=0;i<4;++i)
            {
                plots[i].yAxis->setLabel("功率因素/KVA");
            }
        }
    }
    for (int i=0;i<4;i++)
    {
        plots[i].replot();
        plots[i].yAxis->rescale();
    }
}

void HistoryDataWindow::on_setRangeBtn_clicked()
{
    QDateTime beginDateTime,endDateTime;
    beginDateTime.setDate(ui->beginDate->date());
    beginDateTime.setTime(QTime(ui->beginHour->value(),0,0,0));
    endDateTime.setDate(ui->endDate->date());
    endDateTime.setTime(QTime(ui->endHour->value(),0,0,0));
    range=360;
    dataPro->dataSlicer(beginDateTime,endDateTime,datapoints,range);
    replot(mode,range);
}

void HistoryDataWindow::on_returnBtn_clicked()
{
    this->hide();
}



void HistoryDataWindow::on_sincBtn_2_clicked()
{
    ui->beginHour->setValue(ui->beginHour->value()+1);
    if (ui->beginHour->value()>23)
        ui->beginHour->setValue(23);
}

void HistoryDataWindow::on_sdecBtn_2_clicked()
{
    ui->beginHour->setValue(ui->beginHour->value()-1);
    if (ui->beginHour->value()<0)
        ui->beginHour->setValue(0);

}

void HistoryDataWindow::on_eincBtn_clicked()
{
    ui->endHour->setValue(ui->endHour->value()+1);
    if (ui->endHour->value()>23)
        ui->endHour->setValue(23);
}

void HistoryDataWindow::on_edecBtn_clicked()
{
    ui->endHour->setValue(ui->endHour->value()-1);
    if (ui->endHour->value()<0)
        ui->endHour->setValue(0);
}

