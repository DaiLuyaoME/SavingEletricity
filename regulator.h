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

class Regulator : public QObject
{
    Q_OBJECT
public:
    explicit Regulator(QObject *parent = 0);
    void sendVoltageAtMinPower(DataPoint &voltage);
    void sendAmmeterData(DataPoint& data);
    void beginRegulate();
    void beginTest();
    void closePort();
    void openPort();
    void ack();

signals:
    void regulationOver();//调节结束信号，上位机收到下位机调节结束信息时，产生此信号
    void regulationBegun();//调节开始信号，下位机开始调节电压时，产生此信号
    void testBegun();//节电率测试开始信号
    void testOver();//节电率测试结束信号
    void regulatorError();//下位机错误信号，与下位机通讯产生错误时，产生此信号

public slots:
    void sendVoltageToRegulator(DataPoint &data);
    void parseData();//解析下位机传回的数据
    void startTimeout();
    void handleTimeout();
    void handleSimulationTimeout();

private:
    QSerialPort * regulatorPort;
    QByteArray buffer;
    QTimer * timeoutTimer;
    QTimer * simulationTimer;
    RegulatorInstructionType type;
    int timeoutCount;
    bool gotEnding;

};

#endif // REGULATOR_H
