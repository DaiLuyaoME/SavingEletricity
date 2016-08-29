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
    float getLastPower();//时间单位 s，获取单位时间内的平均功率
    DataPoint getLastData();//读取最新数据
    float getAveragePower(int timeLength);//second  设定时间内的总做功 totalpower 也在这个函数中计算
    float getMinPower(int timeLength);//second
    void setTimeInterval(int interval,int timettype);//设置不同时间间隔
    void setProportion(float proportion);//设置调节限位比例
    DataPoint getMinPowerDataPoint(int timeLength);//搜索最小功率的数据点
    void dataSlicer(QDateTime begin,QDateTime end,QList<DataPoint> &datapoints, int dataamount);
    float getSavingRate();//结算节电率
    void rewritePowerMessage(float *ap, float *tp, float *up, int timeLength);//average total up
signals:
    void ammeterError();//电表故障
    void dataBaseError();//数据库故障
    void regulatorError();//下位机动作故障
    void actionError();//不合理操作
    void regulatorFinish();//下位机操作完成
    void newRealTimeData();//获得新的数据，通知UI绘图
public slots:
    void regulatorAction();//调节下位机操作
private slots:
    void getData();//UI通过dataprocessor获得数据
    void ammeterGetDataError();//电表读数错误
    bool saveData();//将数据存储到database
    void monitorAction();//每隔一段时间进行监控操作
    void regulatorCount();//计算间隔时间内的数据
    void regulatorStart();//下位机动作后开始计时
    void regulatorActionError();//下位机动作错误
private:
    Database *database;//数据库实例
    Regulator *regulator;//下位机动作实例
    Ammeter *ammeter;//电表实例
    QTimer *getDataTimer;//默认每1秒发动一次
    QTimer *saveDataTimer;//默认每10秒发动一次
    QTimer *monitorTimer;//默认每10分钟发动一次
    QTime *RegulatorTime;//regulator调控及时   
    QList<DataPoint> realTimeDataBuffer;//1point/1s,length=3600
    float getMinPowerVoltage(int timeLength);//找到最低功率点的电压值
    float AveragePower;//当前监控点的平均功率
    float TotalPower;//当前时间间隔内的总功率
    float MinPower;//瞬时功率最小值
    float ProporitonLimit;//限位比例
    float SavingRate;//节电率
    int   GetDataTimeInterval;//采样数据间隔 单位：Second
    int   SaveDataTimeInterval;//存储数据间隔  单位：Second
    int   MonitorTimeInterval;//监控数据间隔 单位：Second


};

#endif // DATAPROCESSOR_H
