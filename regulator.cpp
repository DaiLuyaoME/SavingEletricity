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
    type=NoneRegulationType;



}

void Regulator::sendVoltageAtMinPower(DataPoint voltage)
{

}

void Regulator::beginRegulate()
{
    type=WaitForBegining;
    regulatorPort->write("begin");
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
        if (type==WaitForBegining && buffer.data()==HAS_BEGUN)
        {
            timeoutTimer->stop();
            timeoutCount=0;
            emit regulationBegun();
            buffer.clear();
        }
        else if(type==WaitForEnding && buffer.data()==HAS_ENDED)
        {
            regulatorPort->write(OVER_CONFIRM);
            emit regulationOver();//这个信号有多次发送的潜在风险
        }

    }
}

void Regulator::startTimeout()
{
    if(type==WaitForBegining)
    {
        timeoutTimer->start(TIMEOUT_INTERVAL);
    }
}

void Regulator::handleTimeout()
{
    timeoutTimer->stop();
    ++timeoutCount;
    if (type==WaitForBegining)
    {
        if (timeoutCount<MAX_TIMEOUT_COUNT)
        {
            beginRegulate();
        }
        else
        {
            emit regulatorError(WaitForBegining);
            timeoutCount=0;
        }
    }
    else
    {

    }


}
