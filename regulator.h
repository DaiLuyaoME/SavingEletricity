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

#define POWERRATE                100
#define TIMEOUT_INTERVAL         10000
#define MAX_RESEND_COUNT         10
#define MAX_BUFFER_SIZE          15
#define MAX_ENDTIMEOUT           30000



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
    WaitForThersholdBack,
    WaitForStartEnd,
    WaitForShutDownEnd,
    OpenPort
};
enum State
{
    RIDLE,
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
    void sendData(DataPoint voltage, DataPoint newData);
    void beginRegulate();
    void beginTest();
    void closePort();
    void openPort();
    void simulationSaving();
    void startHardware();
    void shutDownHardware();
    void sendThershold(int percentage, DataPoint voltage, DataPoint newData);
    bool isRegulatorFound();
signals:
    void regulationOver();//调节结束信号，上位机收到下位机调节结束信息时，产生此信号
    void regulationBegun();//调节开始信号，下位机开始调节电压时，产生此信号
    void testBegun();//节电率测试开始信号
    void testOver();//节电率测试结束信号
    void regulatorError();//下位机错误信号，与下位机通讯产生错误时，产生此信号
    void regulatorPortNotFound();//没有找到下位机
    void dataSendBack();//下位机接收到数据
    void startBack();
    void shutDownBack();
    void simulationBack();
    void thersholdBack();//阈值返回
    void regulatorTIMEOUT();
    void startOver();
    void shutDownOver();


public slots:
    void parseData();//解析下位机传回的数据
    void handleTimeout();
    void handleEndTimeout();
    void orderClear(regulatorinstruction order);

private:
    QSerialPort * regulatorPort;
    QByteArray buffer;
    QTimer * timeoutTimer;//等待”OK""ER"计时
    QTimer * endtimeTimer;//等待“END"超时计时
    RegulatorInstructionType type;
    RegulatorInstructionType endtype;
    regulatorinstruction DataOrder;
    regulatorinstruction StartOrder;
    regulatorinstruction ShutDownOrder;
    regulatorinstruction SimulationOrder;
    regulatorinstruction ThersholdOrder;
    State r_state;//接收
    int ReSendCount;
    void sendInstruction(regulatorinstruction order);
    char dataIntoString(QByteArray &data, DataPoint &voltage, DataPoint &newdata);
    DataPoint MinVoltageData;
    DataPoint NewVoltageData;
    bool RegulatorFound;

};

#endif // REGULATOR_H
