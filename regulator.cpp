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
    QObject::connect(this,SIGNAL(dataOver()),this,SLOT(dataClear()));
#ifdef SIMULATION
    simulationTimer=new QTimer(this);
    connect(simulationTimer,&QTimer::timeout,this,&Regulator::handleSimulationTimeout);
#endif
    timeoutCount=0;
    gotEnding=false;
    type=IDLE;
}

void Regulator::sendVoltageAtMinPower(DataPoint voltage,DataPoint newData)
{
    char checkcode;
    DataOrder.head = QString("2aeb").toLocal8Bit();
    DataOrder.head = QByteArray::fromHex(DataOrder.head);
    DataOrder.address = QString("02").toLocal8Bit();
    DataOrder.address = QByteArray::fromHex(DataOrder.address);
    DataOrder.commandtype = QString("05").toLocal8Bit();
    DataOrder.commandtype = QByteArray::fromHex(DataOrder.commandtype);
    DataOrder.datalength = QString("18").toLocal8Bit();
    DataOrder.datalength = QByteArray::fromHex(DataOrder.datalength);
    checkcode = dataIntoString(DataOrder.data,voltage,newData);
    checkcode ^= 0x02^0x05^0x18;
    DataOrder.checkcode.append(checkcode);
    DataOrder.tail = QString("ad").toLocal8Bit();
    DataOrder.tail = QByteArray::fromHex(DataOrder.tail);
    //发送指令
    sendInstruction(DataOrder);
    timeoutTimer->start(TIMEOUT_INTERVAL);//开始超时计时
    //状态改变 发送消息
    type = WaitForDataCheckBack;

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
        if(portDescription.contains(QString("USB")))
        {
            regulatorPort->setPort(info);
        }
        else
        {
            emit regulatorPortNotFound();
            return;
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

void Regulator::simulationSaving()
{
    regulatorinstruction temporder;
    QString *tempdata;
    char checkcode;
    temporder.head = QString("2aeb").toLocal8Bit();
    temporder.head = QByteArray::fromHex(temporder.head);
    temporder.address = QString("02").toLocal8Bit();
    temporder.address = QByteArray::fromHex(temporder.address);
    temporder.commandtype = QString("02").toLocal8Bit();
    temporder.commandtype = QByteArray::fromHex(temporder.commandtype);
    temporder.datalength = QString("01").toLocal8Bit();
    temporder.datalength = QByteArray::fromHex(temporder.datalength);
    temporder.data = QString("00").toLocal8Bit();
    temporder.data = QByteArray::fromHex(temporder.data);
    temporder.checkcode = QString("01").toLocal8Bit();
    temporder.checkcode = QByteArray::fromHex(temporder.checkcode);
    temporder.tail = QString("ad").toLocal8Bit();
    temporder.tail = QByteArray::fromHex(temporder.tail);
    //发送指令
    sendInstruction(temporder);
    timeoutTimer->start(TIMEOUT_INTERVAL);
    //状态改变 发送消息
    type = WaitForSimulationBack;

}

void Regulator::shutDownHardware()
{
    regulatorinstruction temporder;
    QString *tempdata;
    char checkcode;
    temporder.head = QString("2aeb").toLocal8Bit();
    temporder.head = QByteArray::fromHex(temporder.head);
    temporder.address = QString("02").toLocal8Bit();
    temporder.address = QByteArray::fromHex(temporder.address);
    temporder.commandtype = QString("03").toLocal8Bit();
    temporder.commandtype = QByteArray::fromHex(temporder.commandtype);
    temporder.datalength = QString("01").toLocal8Bit();
    temporder.datalength = QByteArray::fromHex(temporder.datalength);
    temporder.data = QString("00").toLocal8Bit();
    temporder.data = QByteArray::fromHex(temporder.data);
    temporder.checkcode = QString("00").toLocal8Bit();
    temporder.checkcode = QByteArray::fromHex(temporder.checkcode);
    temporder.tail = QString("ad").toLocal8Bit();
    temporder.tail = QByteArray::fromHex(temporder.tail);
    //发送指令
    sendInstruction(temporder);
    timeoutTimer->start(TIMEOUT_INTERVAL);
    //状态改变 发送消息
    type = WaitForStartBack;

}

void Regulator::startHardware()
{
    regulatorinstruction temporder;
    QString *tempdata;
    char checkcode;
    temporder.head = QString("2aeb").toLocal8Bit();
    temporder.head = QByteArray::fromHex(temporder.head);
    temporder.address = QString("02").toLocal8Bit();
    temporder.address = QByteArray::fromHex(temporder.address);
    temporder.commandtype = QString("04").toLocal8Bit();
    temporder.commandtype = QByteArray::fromHex(temporder.commandtype);
    temporder.datalength = QString("01").toLocal8Bit();
    temporder.datalength = QByteArray::fromHex(temporder.datalength);
    temporder.data = QString("00").toLocal8Bit();
    temporder.data = QByteArray::fromHex(temporder.data);
    temporder.checkcode = QString("07").toLocal8Bit();
    temporder.checkcode = QByteArray::fromHex(temporder.checkcode);
    temporder.tail = QString("ad").toLocal8Bit();
    temporder.tail = QByteArray::fromHex(temporder.tail);
    //发送指令
    sendInstruction(temporder);
    timeoutTimer->start(TIMEOUT_INTERVAL);
    //状态改变 发送消息
    type = WaitForShutDownBack;

}

void Regulator::sendVoltageToRegulator(DataPoint &data)
{

}
/*
 * 解析数据 主要是收到回复的 “OK" "ER"要重发
*/
void Regulator::parseData()
{
    QByteArray tempbuffer;
    //qDebug()<<"before readall"<<regulatorPort->peek(2);
    tempbuffer.append(regulatorPort->readAll());
    //qDebug()<<"after readall"<<regulatorPort->peek(2);
    switch (r_state)
    {
    case RIDEL:
        if(tempbuffer.contains("O"))
        {
            r_state = RM_O;
        }
        else if(tempbuffer.contains("E"))
        {
            r_state = RM_E;
        }
        break;
    case RM_O:
        if(tempbuffer.contains("K"))
        {
            r_state = RIDEL;
            switch (type)
            {
            case WaitForStartBack:
                //emit testBegun();
                type = IDLE;
                timeoutTimer->stop();//停止延时计时
                break;
            case WaitForShutDownBack:
                type = IDLE;
                //emit regulationBegun();
                timeoutTimer->stop();//停止延时计时
                break;
            case WaitForSimulationBack:
                //emit regulationOver();
                type = IDLE;
                timeoutTimer->stop();
                break;
            case WaitForDataCheckBack:
                emit dataOver();
                type = IDLE;
                timeoutTimer->stop();
                break;
            default:
                break;
            }
        }
        break;
    case RM_E://回复ER 重发
        if(tempbuffer.contains("R"))
        {
            r_state = RIDEL;
            switch (type)
            {
            case WaitForSimulationBack:
                //emit regulationOver();
                timeoutTimer->stop();//停止延时计时
                simulationSaving();
                break;
            case WaitForStartBack:
                //emit testOver();
                timeoutTimer->stop();//停止延时计时
                startHardware();
                break;
            case WaitForShutDownBack:
                //
                timeoutTimer->stop();
                shutDownHardware();
                break;
            case WaitForDataCheckBack:
                //
                timeoutTimer->stop();
                sendInstruction(DataOrder);
                break;
            default:
                break;
            }
        }
        break;
    default:
        break;
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
    ++timeoutCount;
    if(timeoutCount >= MAX_TIMEOUT_COUNT)
    {
        emit regulatorTIMEOUT();
        timeoutTimer->stop();
    }

}

void Regulator::dataClear()
{
    DataOrder.head.clear();
    DataOrder.address.clear();
    DataOrder.commandtype.clear();
    DataOrder.datalength.clear();
    DataOrder.data.clear();
    DataOrder.checkcode.clear();
    DataOrder.tail.clear();
}
#ifdef SIMULATION
void Regulator::handleSimulationTimeout()
{
    simulationTimer->stop();
    if(type==WaitForRegulationEnding)
    {
        emit regulationOver();
    }
    else if(type==WaitForTestEnding)
    {
        emit testOver();
    }

}
#endif
void Regulator::sendInstruction(regulatorinstruction order)
{
    regulatorPort->write(order.head);
    regulatorPort->write(order.address);
    regulatorPort->write(order.commandtype);
    regulatorPort->write(order.datalength);
    regulatorPort->write(order.data);
    regulatorPort->write(order.checkcode);
    regulatorPort->write(order.tail);
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
    tempbuffer = int(newdata.epa);//
    data.append(((tempbuffer&0xff00)>>8));
    checkcode ^= data.at(data.size()-1);
    data.append(((tempbuffer&0xff)));
    checkcode ^= data.at(data.size()-1);
    tempbuffer = int(newdata.epb);//
    data.append(((tempbuffer&0xff00)>>8));
    checkcode ^= data.at(data.size()-1);
    data.append(((tempbuffer&0xff)));
    checkcode ^= data.at(data.size()-1);
    tempbuffer = int(newdata.epc);//
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





