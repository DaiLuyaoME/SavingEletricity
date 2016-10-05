#include "regulator.h"
#include <stdio.h>


Regulator::Regulator(QObject *parent) : QObject(parent)
{
    openPort();
    timeoutTimer = new QTimer(this);
    endtimeTimer = new QTimer(this);
    connect(timeoutTimer,&QTimer::timeout,this,&Regulator::handleTimeout);
    QObject::connect(endtimeTimer,SIGNAL(timeout()),this,SLOT(handleEndTimeout()));
    ReSendCount = 0;
    type=IDLE;
    endtype=IDLE;
    r_state = RIDLE;
}



void Regulator::beginRegulate()
{

}

void Regulator::beginTest()
{

}

//具体的串口产生需要根据实际情况进行调整
void Regulator::openPort()
{
    regulatorPort=new QSerialPort(this);
    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        QString portDescription=info.description();

        if(portDescription.contains(QString("CH340")))
        {
            qDebug()<<"regulator found";
            RegulatorFound = true;
            regulatorPort->setPort(info);
            regulatorPort->setBaudRate(QSerialPort::Baud9600);
            regulatorPort->setDataBits(QSerialPort::Data8);
            regulatorPort->setParity(QSerialPort::NoParity);
            regulatorPort->setStopBits(QSerialPort::OneStop);
            regulatorPort->setFlowControl(QSerialPort::NoFlowControl);
            regulatorPort->open(QIODevice::ReadWrite);
            connect(regulatorPort,&QSerialPort::readyRead,this,&Regulator::parseData);
            return;
        }
    }
    RegulatorFound = false;
    emit regulatorPortNotFound();
    return;

}

void Regulator::sendData(DataPoint voltage,DataPoint newData)
{
    if(type != IDLE)return;    //指令冲突处理
    orderClear(DataOrder);
    char checkcode;
    DataOrder.head = QString("2aeb").toLocal8Bit();
    DataOrder.head = QByteArray::fromHex(DataOrder.head);
    DataOrder.address = QString("02").toLocal8Bit();
    DataOrder.address = QByteArray::fromHex(DataOrder.address);
    DataOrder.commandtype = QString("05").toLocal8Bit();
    DataOrder.commandtype = QByteArray::fromHex(DataOrder.commandtype);
    DataOrder.datalength = QString("18").toLocal8Bit();//
    DataOrder.datalength = QByteArray::fromHex(DataOrder.datalength);
    checkcode = dataIntoString(DataOrder.data,voltage,newData);
    checkcode ^= 0x02^0x05^0x18;
    DataOrder.checkcode.append(checkcode);
    DataOrder.tail = QString("ad").toLocal8Bit();
    DataOrder.tail = QByteArray::fromHex(DataOrder.tail);
    //发送指令
    sendInstruction(DataOrder);
    //状态改变 发送消息
    type = WaitForDataCheckBack;

}

//节能模拟指令
void Regulator::simulationSaving()
{
    if(type != IDLE)return;
    orderClear(SimulationOrder);
    SimulationOrder.head = QString("2aeb").toLocal8Bit();
    SimulationOrder.head = QByteArray::fromHex(SimulationOrder.head);
    SimulationOrder.address = QString("02").toLocal8Bit();
    SimulationOrder.address = QByteArray::fromHex(SimulationOrder.address);
    SimulationOrder.commandtype = QString("02").toLocal8Bit();
    SimulationOrder.commandtype = QByteArray::fromHex(SimulationOrder.commandtype);
    SimulationOrder.datalength = QString("01").toLocal8Bit();
    SimulationOrder.datalength = QByteArray::fromHex(SimulationOrder.datalength);
    SimulationOrder.data = QString("00").toLocal8Bit();
    SimulationOrder.data = QByteArray::fromHex(SimulationOrder.data);
    SimulationOrder.checkcode = QString("01").toLocal8Bit();
    SimulationOrder.checkcode = QByteArray::fromHex(SimulationOrder.checkcode);
    SimulationOrder.tail = QString("ad").toLocal8Bit();
    SimulationOrder.tail = QByteArray::fromHex(SimulationOrder.tail);
    //发送指令
    sendInstruction(SimulationOrder);
    //状态改变 发送消息
    type = WaitForSimulationBack;

}
//软关机指令
void Regulator::shutDownHardware()
{
    if(type != IDLE)return;
    orderClear(ShutDownOrder);
    endtype = WaitForShutDownEnd;
    ShutDownOrder.head = QString("2aeb").toLocal8Bit();
    ShutDownOrder.head = QByteArray::fromHex(ShutDownOrder.head);
    ShutDownOrder.address = QString("02").toLocal8Bit();
    ShutDownOrder.address = QByteArray::fromHex(ShutDownOrder.address);
    ShutDownOrder.commandtype = QString("03").toLocal8Bit();
    ShutDownOrder.commandtype = QByteArray::fromHex(ShutDownOrder.commandtype);
    ShutDownOrder.datalength = QString("01").toLocal8Bit();
    ShutDownOrder.datalength = QByteArray::fromHex(ShutDownOrder.datalength);
    ShutDownOrder.data = QString("00").toLocal8Bit();
    ShutDownOrder.data = QByteArray::fromHex(ShutDownOrder.data);
    ShutDownOrder.checkcode = QString("00").toLocal8Bit();
    ShutDownOrder.checkcode = QByteArray::fromHex(ShutDownOrder.checkcode);
    ShutDownOrder.tail = QString("ad").toLocal8Bit();
    ShutDownOrder.tail = QByteArray::fromHex(ShutDownOrder.tail);
    //发送指令
    sendInstruction(ShutDownOrder);
    //状态改变 发送消息
    type = WaitForShutDownBack;

}
//发送阈值指令
void Regulator::sendThershold(int percentage, DataPoint voltage, DataPoint newData)
{
    if(type != IDLE)return;
    MinVoltageData = voltage;
    NewVoltageData = newData;
    orderClear(ThersholdOrder);
    char checkcode;
    ThersholdOrder.head = QString("2aeb").toLocal8Bit();
    ThersholdOrder.head = QByteArray::fromHex(ThersholdOrder.head);
    ThersholdOrder.address = QString("02").toLocal8Bit();
    ThersholdOrder.address = QByteArray::fromHex(ThersholdOrder.address);
    ThersholdOrder.commandtype = QString("06").toLocal8Bit();
    ThersholdOrder.commandtype = QByteArray::fromHex(ThersholdOrder.commandtype);
    ThersholdOrder.datalength = QString("01").toLocal8Bit();
    ThersholdOrder.datalength = QByteArray::fromHex(ThersholdOrder.datalength);
    ThersholdOrder.data.append(percentage);
    checkcode=0x02^0x06^0x01^percentage;
    ThersholdOrder.checkcode.append(checkcode);
    ThersholdOrder.tail = QString("ad").toLocal8Bit();
    ThersholdOrder.tail = QByteArray::fromHex(ThersholdOrder.tail);
    //发送指令
    sendInstruction(ThersholdOrder);
    //状态改变 发送消息
    type = WaitForThersholdBack;

}

bool Regulator::isRegulatorFound()
{
    return RegulatorFound;
}
//软开机指令
void Regulator::startHardware()
{
    if(type != IDLE)return;
    orderClear(StartOrder);
    endtype = WaitForStartEnd;
    StartOrder.head = QString("2aeb").toLocal8Bit();
    StartOrder.head = QByteArray::fromHex(StartOrder.head);
    StartOrder.address = QString("02").toLocal8Bit();
    StartOrder.address = QByteArray::fromHex(StartOrder.address);
    StartOrder.commandtype = QString("04").toLocal8Bit();
    StartOrder.commandtype = QByteArray::fromHex(StartOrder.commandtype);
    StartOrder.datalength = QString("01").toLocal8Bit();
    StartOrder.datalength = QByteArray::fromHex(StartOrder.datalength);
    StartOrder.data = QString("00").toLocal8Bit();
    StartOrder.data = QByteArray::fromHex(StartOrder.data);
    StartOrder.checkcode = QString("07").toLocal8Bit();
    StartOrder.checkcode = QByteArray::fromHex(StartOrder.checkcode);
    StartOrder.tail = QString("ad").toLocal8Bit();
    StartOrder.tail = QByteArray::fromHex(StartOrder.tail);
    //发送指令
    sendInstruction(StartOrder);
    //状态改变 发送消息
    type = WaitForStartBack;
}

/*
 * 解析数据 主要是收到回复的 “OK" "ER"要重发
 * 注意每次readytoread的时候已经接受了完整的数据
*/
void Regulator::parseData()
{
    QByteArray tempbuffer=regulatorPort->readAll();
    // qDebug()<<tempbuffer.length();
    //qDebug()<<"before readall"<<regulatorPort->peek(4);
    //tempbuffer.append(regulatorPort->readAll());
    //printf("back data is %c\r\n",tempbuffer.at(0));
    //printf("back data 2 is %c\r\n",tempbuffer.at(tempbuffer.size()-1));
    qDebug()<<"after readall"<<tempbuffer.at(0)<<tempbuffer.at(1);

//    switch (r_state)
//    {
//    case RIDEL:
//        if(tempbuffer.contains("O"))
//        {
//            r_state = RM_O;
//        }
//        else if(tempbuffer.contains("E"))
//        {
//            r_state = RM_E;
//        }
//        break;
//    case RM_O:
        if(tempbuffer.contains("OK"))
        {
            ReSendCount = 0;
            timeoutTimer->stop();
            r_state = RIDLE;
            switch (type)
            {
            case WaitForStartBack:
                type = IDLE;
                endtimeTimer->start(MAX_ENDTIMEOUT);
                emit startBack();
                break;
            case WaitForShutDownBack:
                type = IDLE;
                endtimeTimer->start(MAX_ENDTIMEOUT);
                emit shutDownBack();
                break;
            case WaitForSimulationBack:
                type = IDLE;
                emit simulationBack();
                break;
            case WaitForDataCheckBack:
                type = IDLE;
                emit dataSendBack();
                break;
            case WaitForThersholdBack:
                emit thersholdBack();
                qDebug()<<"2";
                type = IDLE;
                sendData(MinVoltageData,NewVoltageData);
                break;
            default:
                break;
            }

        }
//        break;
//    case RM_E://回复ER 重发
        if(tempbuffer.contains("ER"))
        {
            r_state = RIDLE;
            timeoutTimer->stop();//停止延时计时
            ReSendCount += 1;
            if(ReSendCount > MAX_RESEND_COUNT)
            {

                //ReSendCount = 0;
                emit regulatorError();
                qDebug()<<"error emit";
                return;
            }
            qDebug()<<"er coount"<<ReSendCount;
            switch (type)
            {
            case WaitForSimulationBack:
                sendInstruction(SimulationOrder);
                break;
            case WaitForStartBack:
                sendInstruction(StartOrder);
                break;
            case WaitForShutDownBack:
                sendInstruction(ShutDownOrder);
                break;
            case WaitForDataCheckBack:
                sendInstruction(DataOrder);
            case WaitForThersholdBack:
                sendInstruction(ThersholdOrder);
                break;
            default:
                break;
            }

        }
        if(tempbuffer.contains("END"))
        {
            endtimeTimer->stop();
            switch (endtype)
            {
            case WaitForStartEnd:
                endtype = IDLE;
                emit startOver();
                break;
            case WaitForShutDownEnd:
                endtype = IDLE;
                emit shutDownOver();
                break;
            default:
                break;

            }
        }


}


void Regulator::handleTimeout()
{
        timeoutTimer->stop();
        emit regulatorError();
}

void Regulator::handleEndTimeout()
{
    endtimeTimer->stop();
    emit regulatorError();
}

void Regulator::orderClear(regulatorinstruction order)
{
    order.head.clear();
    order.address.clear();
    order.commandtype.clear();
    order.datalength.clear();
    order.data.clear();
    order.checkcode.clear();
    order.tail.clear();
}

void Regulator::sendInstruction(regulatorinstruction order)
{
    regulatorPort->write(order.head);
    regulatorPort->write(order.address);
    regulatorPort->write(order.commandtype);
    regulatorPort->write(order.datalength);
    regulatorPort->write(order.data);
    regulatorPort->write(order.checkcode);
    regulatorPort->write(order.tail);
    timeoutTimer->start(TIMEOUT_INTERVAL);
}
//返回data异或
char Regulator::dataIntoString(QByteArray &data, DataPoint &voltage, DataPoint &newdata)
{
    int tempbuffer;
    char checkcode = 0;
    tempbuffer = int(newdata.va * 10);//
    data.append(((tempbuffer&0xff00)>>8));
    checkcode = data.at(data.size()-1);
    data.append(((tempbuffer&0xff)));
    checkcode ^= data.at(data.size()-1);
    tempbuffer = int(newdata.vb * 10);//
    data.append(((tempbuffer&0xff00)>>8));
    checkcode ^= data.at(data.size()-1);
    data.append(((tempbuffer&0xff)));
    checkcode ^= data.at(data.size()-1);
    tempbuffer = int(newdata.vc * 10);//
    data.append(((tempbuffer&0xff00)>>8));
    checkcode ^= data.at(data.size()-1);
    data.append(((tempbuffer&0xff)));
    checkcode ^= data.at(data.size()-1);
    tempbuffer = int(newdata.ia * 10);//
    data.append(((tempbuffer&0xff00)>>8));
    checkcode ^= data.at(data.size()-1);
    data.append(((tempbuffer&0xff)));
    checkcode ^= data.at(data.size()-1);
    tempbuffer = int(newdata.ib * 10);//
    data.append(((tempbuffer&0xff00)>>8));
    checkcode ^= data.at(data.size()-1);
    data.append(((tempbuffer&0xff)));
    checkcode ^= data.at(data.size()-1);
    tempbuffer = int(newdata.ic * 10);//
    data.append(((tempbuffer&0xff00)>>8));
    checkcode ^= data.at(data.size()-1);
    data.append(((tempbuffer&0xff)));
    checkcode ^= data.at(data.size()-1);
    tempbuffer = int(newdata.epa / POWERRATE);//
    data.append(((tempbuffer&0xff00)>>8));
    checkcode ^= data.at(data.size()-1);
    data.append(((tempbuffer&0xff)));
    checkcode ^= data.at(data.size()-1);
    tempbuffer = int(newdata.epb / POWERRATE);//
    data.append(((tempbuffer&0xff00)>>8));
    checkcode ^= data.at(data.size()-1);
    data.append(((tempbuffer&0xff)));
    checkcode ^= data.at(data.size()-1);
    tempbuffer = int(newdata.epc / POWERRATE);//
    data.append(((tempbuffer&0xff00)>>8));
    checkcode ^= data.at(data.size()-1);
    data.append(((tempbuffer&0xff)));
    checkcode ^= data.at(data.size()-1);
    tempbuffer = int(voltage.va * 10);//
    data.append(((tempbuffer&0xff00)>>8));
    checkcode ^= data.at(data.size()-1);
    data.append(((tempbuffer&0xff)));
    checkcode ^= data.at(data.size()-1);
    tempbuffer = int(voltage.vb * 10);//
    data.append(((tempbuffer&0xff00)>>8));
    checkcode ^= data.at(data.size()-1);
    data.append(((tempbuffer&0xff)));
    checkcode ^= data.at(data.size()-1);
    tempbuffer = int(voltage.vc * 10);//
    data.append(((tempbuffer&0xff00)>>8));
    checkcode ^= data.at(data.size()-1);
    data.append(((tempbuffer&0xff)));
    checkcode ^= data.at(data.size()-1);

    return checkcode;
    //qDebug<<"data is "<<(unsigned char)data.data();
}





