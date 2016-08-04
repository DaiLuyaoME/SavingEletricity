#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <QObject>
#include <datapoint.h>
#include <database.h>
#include <regulator.h>
#include <ammeter.h>
#include <QTimer>
#include <QList>
#include <QTime>

class DataProcessor : public QObject
{
    Q_OBJECT
public:
    explicit DataProcessor(QObject *parent = 0);

signals:
    void ammeterError();
    void dataBaseError();
    void regulatorError();

public slots:
    void getData();//read data from ammeter
    void saveData();//save data to database
    void startMonitor();
    void stopMonitor();
    QList<DataPoint> dataSlicer(QTime begin,QTime end);
    void setTimeInterval();
    void getAveragePower(int timeLength);//second
    void getMinPower(int timeLength);//second

private:
    Database *database;
    Regulator *regulator;
    Ammeter *ammeter;//电表实例
    QTimer *getDataTimer;//每秒发动一次
    QTimer *saveDataTimer;//默认每10秒发动一次
    QTimer *monitorTimer;//每10分钟发动一次
    QList<DataPoint> realTimeDataBuffer;//1point/1s,length=3600

};

#endif // DATAPROCESSOR_H
