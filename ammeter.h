/**
 * Ammeter class, responsible for ammeter operation such as data reaing(voltage, current, effective power, etc.)
 *
 * */

#ifndef AMMETER_H
#define AMMETER_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <datapoint.h>
#include <QTimer>
#include <QtDebug>
//#define SIMULATION
struct instruction
{
    QByteArray voltageInstruction;
    QByteArray currentInstruction;
    QByteArray effectivePowerInstruction;
    QByteArray reactivePowerInstruction;
    QByteArray apparentPowerInstruction;
    QByteArray powerFactorInstruction;
};

enum readType
{
    NoneType=0,
    ReadVoltage,
    ReadCurrent,
    ReadEffectivePower,
    ReadReactivePower,
    ReadApparentPower,
    ReadPowerFactor
};

class Ammeter : public QObject
{
    Q_OBJECT
public:
    explicit Ammeter(QObject *parent = 0);
//    int readLatestData(DataPoint & data);//return -1 if failed
    DataPoint getData();//get latest data from ammeter
private:
    void initPort();
    void initInstruction();
    void initConnections();
    void initData();
    datatype minus33(QByteArray data);
signals:
    void voltageDataGot();
    void currentDataGot();
    void effectivePowerDataGot();
    void reactivePowerDataGot();
    void apparentPowerDataGot();
    void powerFactorDataGot();
    void getDataOver();
    void ammeterError();

public slots:
    void parseData();
    void startTimeoutTimer();
    void handleTimeOut();
    void getVoltage();
    void getCurrent();
    void getEffectivePower();
    void getReactivePower();
    void getApparentPower();
    void getPowerFactor();
    void startReadTimer();
#ifdef SIMULATION
    void changeCoeffcient(float value);
#endif

private:
    DataPoint latestData;
    instruction m_instruction;
    QByteArray buffer;
    QSerialPort * portReadAndWrite;
    readType m_readType;
    QTimer * timeout;
    QTimer * readTimer;
    int failureCount;
    float coefficient;
};

#endif // AMMETER_H
