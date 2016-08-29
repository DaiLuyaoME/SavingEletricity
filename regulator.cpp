#include "regulator.h"
#define TIMEOUT_INTERVAL 1500
#define MAX_TIMEOUT_COUNT  3
#define MAX_BUFFER_SIZE 15
#define BEGIN_REGULATE "begin"
#define HAS_BEGUN "hasbegun"
#define HAS_ENDED "over"
#define OVER_CONFIRM "overconfirm"
Regulator::Regulator(QObject *parent) : QObject(parent)
{
    openPort();
    timeoutTimer=new QTimer(this);
    connect(timeoutTimer,&QTimer::timeout,this,&Regulator::handleTimeout);
    timeoutCount=0;
    gotEnding=false;
    type=NoneRegulationType;
}

void Regulator::sendVoltageAtMinPower(DataPoint &voltage)
{
    QString temp=QString("PMVA%1PMVB%2PMVC%3").arg(voltage.va,0,'f',1).arg(voltage.vb,0,'f',1).arg(voltage.vc,0,'f',1);
    regulatorPort->write(temp.toLocal8Bit()); 
    qDebug()<<"the voltage at minpower is "<<temp;
    qDebug()<<"data to write "<<temp.toLocal8Bit(); 


}

///数据不要忘了加互感器参数！！！！
void Regulator::sendAmmeterData(DataPoint& data)
{
    QString temp=QString("VA%1VB%2VC%3").arg(data.va,0,'f',1).arg(data.vb,0,'f',1).arg(data.vc,0,'f',1);
    qDebug()<<"voltage:"<<temp;
    qDebug()<<"voltage data to write:"<<temp.toLocal8Bit();
    regulatorPort->write(temp.toLocal8Bit());
    temp=QString("IA%1IB%2IC%3").arg(data.ia,0,'f',3).arg(data.ib,0,'f',3).arg(data.ic,0,'f',3);
    qDebug()<<"current:"<<temp;
    qDebug()<<"current data to write:"<<temp.toLocal8Bit();
    regulatorPort->write(temp.toLocal8Bit());
    temp=QString("PA%1PB%2PC%3PS%4").arg(data.epa,0,'f',3).arg(data.epb,0,'f',3).arg(data.epc,0,'f',3).arg(data.eps,0,'f',3);
    qDebug()<<"effective power："<<temp;
    qDebug()<<"effective power data to write:"<<temp.toLocal8Bit();
    regulatorPort->write(temp.toLocal8Bit());

}

void Regulator::beginRegulate()
{
    type=WaitForRegulationBegining;
    buffer.clear();
    gotEnding=false;
    regulatorPort->write("BR");
}

void Regulator::beginTest()
{
    type=WaitForTestBegining;
    buffer.clear();
    gotEnding=false;
    regulatorPort->write("BT");
}

//具体的串口产生需要根据实际情况进行调整
void Regulator::openPort()
{
    regulatorPort=new QSerialPort(this);
    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        QString portDescription=info.description();
        if(portDescription.contains(QString("USB")))
        {
            regulatorPort->setPort(info);
        }

    }
    regulatorPort->setBaudRate(QSerialPort::Baud9600);
    regulatorPort->setDataBits(QSerialPort::Data8);
    regulatorPort->setParity(QSerialPort::EvenParity);
    regulatorPort->setStopBits(QSerialPort::OneStop);
    regulatorPort->setFlowControl(QSerialPort::NoFlowControl);
    regulatorPort->open(QIODevice::ReadWrite);
    connect(regulatorPort,&QSerialPort::readyRead,this,&Regulator::parseData);
    connect(regulatorPort,&QSerialPort::bytesWritten,this,&Regulator::startTimeout);
}

void Regulator::ack()
{

}

void Regulator::sendVoltageToRegulator(DataPoint &data)
{

}

void Regulator::parseData()
{
    buffer.append(regulatorPort->readAll());
    if(buffer.size()>MAX_BUFFER_SIZE)
    {
        buffer.clear();
    }
    else
    {
        if(buffer.contains("OK"))
        {
            timeoutCount=0;
            timeoutTimer->stop();
            buffer.clear();
            if(type==WaitForRegulationBegining)
            {
                emit regulationBegun();
                type=WaitForRegulationEnding;
            }
            else if(type==WaitForTestBegining)
            {
                emit testBegun();
                type=WaitForTestEnding;
            }
        }
        else if(buffer.contains("ER"))
        {
            if(gotEnding==false)
            {
                gotEnding=true;
                emit regulationOver();
            }
            else
            {

            }
        }
        else if(buffer.contains("ET"))
        {
            if(gotEnding==false)
            {
                gotEnding=true;
                emit testOver();
            }
            else
            {

            }
        }
    }
}

void Regulator::startTimeout()
{
    if(type==WaitForRegulationBegining || type==WaitForTestBegining)
    {
        timeoutTimer->start(TIMEOUT_INTERVAL);
    }
}

void Regulator::handleTimeout()
{
    timeoutTimer->stop();
    ++timeoutCount;
    if (type==WaitForRegulationBegining)
    {
        if (timeoutCount<MAX_TIMEOUT_COUNT)
        {
            beginRegulate();
        }
        else
        {
            emit regulatorError();
            timeoutCount=0;
        }
    }
    else if(type==WaitForTestBegining)
    {
        if (timeoutCount<MAX_TIMEOUT_COUNT)
        {
            beginTest();
        }
        else
        {
            emit regulatorError();
            timeoutCount=0;
        }
    }
}
