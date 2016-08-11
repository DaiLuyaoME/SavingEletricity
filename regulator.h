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


class Regulator : public QObject
{
    Q_OBJECT
public:
    explicit Regulator(QObject *parent = 0);
    //    void sendVotageAtMinPower(datatype voltage);
    void beginRegulate();
    void closePort();
    void openPort();
    void ack();

signals:
    void regulationOver();//调节结束信号，上位机收到下位机调节结束信息时，产生此信号
    void regulationBegun();//调节开始信号，下位机开始调节电压时，产生此信号
    void regulatorError(RegulatorInstructionType errorType);

public slots:
    void sendVoltageToRegulator(DataPoint &data);
    void parseData();//解析下位机传回的数据
    void startTimeout();
    void handleTimeout();

private:
    QSerialPort * regulatorPort;
    QByteArray buffer;
    QTimer * timeoutTimer;
    RegulatorInstructionType type;
    int timeoutCount;

};

#endif // REGULATOR_H
