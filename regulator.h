/**
 * Regulator class，namely the lower computer, responsible for the communication with regulator(lower computer)
 * */
#ifndef REGULATOR_H
#define REGULATOR_H

#include <QObject>
#include <datapoint.h>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QtDebug>
//#define SIMULATION
struct regulatorinstruction
{
    QByteArray head;//帧头
    QByteArray address;//通讯地址
    QByteArray commandtype;//指令类型
    QByteArray datalength;//数据长度
    QByteArray data;//数据
    QByteArray checkcode;//校验码
    QByteArray tail;//尾部
};
enum RegulatorInstructionType
{
    IDLE,
    WaitForRegulationBegining,
    WaitForRegulationEnding,
    WaitForTestBegining,
    WaitForTestEnding,
    WaitForSimulationBack,
    WaitForStartBack,
    WaitForShutDownBack,
    WaitForDataCheckBack,
    OpenPort
};
enum State
{
    RIDEL,
    RM_O,
    RM_K,
    RM_E,
    RM_R,
    REND
};

class Regulator : public QObject
{
    Q_OBJECT
public:
    explicit Regulator(QObject *parent = 0);
    void sendVoltageAtMinPower(DataPoint voltage, DataPoint newData);
    void beginRegulate();
    void beginTest();
    void closePort();
    void openPort();
    void ack();
    void simulationSaving();
    void startHardware();
    void shutDownHardware();
signals:
    void regulationOver();//调节结束信号，上位机收到下位机调节结束信息时，产生此信号
    void regulationBegun();//调节开始信号，下位机开始调节电压时，产生此信号
    void testBegun();//节电率测试开始信号
    void testOver();//节电率测试结束信号
    void regulatorError();//下位机错误信号，与下位机通讯产生错误时，产生此信号
    void regulatorPortNotFound();//没有找到下位机
    void dataOver();//下位机接收到数据
    void regulatorTIMEOUT();

public slots:
    void sendVoltageToRegulator(DataPoint &data);
    void parseData();//解析下位机传回的数据
    void startTimeout();
    void handleTimeout();
    void dataClear();
#ifdef SIMULATION
    void handleSimulationTimeout();
#endif

private:
    QSerialPort * regulatorPort;
    QByteArray buffer;
    QTimer * timeoutTimer;
#ifdef SIMULATION
    QTimer * simulationTimer;
#endif
    RegulatorInstructionType type;
    regulatorinstruction DataOrder;
    regulatorinstruction StartOrder;
    regulatorinstruction ShutDownOrder;
    regulatorinstruction simulationOrder;
    State r_state;//接收
    int timeoutCount;
    bool gotEnding;//??
    void sendInstruction(regulatorinstruction order);
    char dataIntoString(QByteArray &data, DataPoint &voltage, DataPoint &newdata);

};

#endif // REGULATOR_H
