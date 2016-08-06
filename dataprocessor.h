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
    ~DataProcessor();

signals:
    void ammeterError();//电表故障
    void dataBaseError();//数据库故障
    void regulatorError();//下位机动作故障
    void actionError();//不合理操作
    /*比如
    *1、刚启动就进行下位机动作，此时数据没有采集完成————最好设定启动一段时间后才能手动操作
    */


public slots:
    void getData();//read data from ammeter
    void saveData();//save data to database
    void startMonitor();
    void stopMonitor();
    QList<DataPoint> dataSlicer(QTime begin,QTime end);
    void setTimeInterval(int interval,int timettype);//设置不同时间间隔
    void getAveragePower(int timeLength);//second  设定时间内的总做功 totalpower 也在这个函数中计算
    float getMinPower(int timeLength);//second

    float OldAveragePower;//上一个监控点的平均功率
    float OldTotalPower;//上一个监控点间隔时间内的总功率

    //单位：Second
    int   GetDataTimeInterval;//采样数据间隔
    int   SaveDataTimeInterval;//存储数据间隔
    int   MoniterTimeInterval;//监控数据间隔



private:
    Database *database;
    Regulator *regulator;
    Ammeter *ammeter;//电表实例
    QTimer *getDataTimer;//每秒发动一次
    QTimer *saveDataTimer;//默认每10秒发动一次
    QTimer *monitorTimer;//每10分钟发动一次
    QList<DataPoint> realTimeDataBuffer;//1point/1s,length=3600

    float AveragePower;//当前监控点的平均功率
    float TotalPower;//当前时间间隔内的总功率
    int   MinMomentPower;//瞬时功率最小值

    void moniter(int timelength);//启动监控

};

#endif // DATAPROCESSOR_H
