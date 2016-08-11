#include "regulator.h"

Regulator::Regulator(QObject *parent) : QObject(parent)
{

}

void Regulator::sendVoltageAtMinPower(datatype voltage)
{

}

void Regulator::beginRegulate()
{

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
//    connect(regulatorPort,&QSerialPort::bytesWritten,this,&Regulator::startTimeoutTimer);

}

void Regulator::ack()
{

}

void Regulator::sendVoltageToRegulator(DataPoint &data)
{

}

void Regulator::parseData()
{

}
