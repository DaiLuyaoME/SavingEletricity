#include "dataprocessor.h"
#include "math.h"
#define REALTIMEDATABUFFERCOUNT 3600//realTimeDataBuffer 数据个数
#define GETDATATIMEDEFAULT      1//默认读取数据间隔 单位：second
#define SAVEDATATIMEDEFALT      1//默认存储数据间隔 单位：second
#define MONITORTIMEDEFALT       10//默认监控计算间隔 单位：second
#define PROPORTIONDEFAULT       4.0//默认调节限制比例
#define ERROR                   -1//报错回传参数

DataProcessor::DataProcessor(QObject *parent) : QObject(parent)
{
	/*实例初始化*/
    database = new Database(this);//数据库实例
	ammeter = new Ammeter(this);//电表实例
	regulator  = new Regulator(this);//下位机实例
    saveDataTimer = new QTimer(this);//存储数据计时器
    monitorTimer = new QTimer(this);//电表监控计时器
    /*参数初始化*/
    ProporitonLimit       = PROPORTIONDEFAULT;
    AveragePower          = 0.0;
    TotalPower            = 0.0;
    GetDataTimeInterval   = GETDATATIMEDEFAULT;
    SaveDataTimeInterval  = SAVEDATATIMEDEFALT;
    MonitorTimeInterval   = MONITORTIMEDEFALT;
    /*读取数据*/
    QObject::connect(ammeter,SIGNAL(getDataOver()),this,SLOT(getData()));
    QObject::connect(ammeter,SIGNAL(ammeterError()),this,SLOT(ammeterGetDataError()));
    QObject::connect(ammeter,SIGNAL(ammeterNotFound()),this,SLOT(ammeterGetDataError()));
    if(ammeter->isAmmeterFound())AmmeterError=false;
    if(!ammeter->isAmmeterFound())ammeterGetDataError();
    if(regulator->isRegulatorFound())RegulatorError=false;
    if(!regulator->isRegulatorFound())regulatorConnectError();
    /*存储数据*/
    QObject::connect(saveDataTimer,SIGNAL(timeout()),this,SLOT(saveData()));
    saveDataTimer->start(SaveDataTimeInterval*1000);//默认10s一次
    /*监控一次*/
    QObject::connect(monitorTimer,SIGNAL(timeout()),this,SLOT(monitorAction()));
    monitorTimer->start(MonitorTimeInterval*1000);//默认600s一次
   /*regulator 软关机开始，节电测试开始*/
    QObject::connect(regulator,SIGNAL(shutDownBack()),this,SLOT(testStart()));
    QObject::connect(regulator,SIGNAL(shutDownOver()),this,SLOT(testCount()));
    QObject::connect(regulator,SIGNAL(regulatorError()),this,SLOT(regulatorConnectError()));


}
DataProcessor::~DataProcessor()
{

}
/*
*通过电表ammeter读取数据 队列头是最新数据
*/
void DataProcessor::getData()
{
    DataPoint tempdata;
        tempdata = ammeter->getData();//读取数据
        if(realTimeDataBuffer.size()>=REALTIMEDATABUFFERCOUNT)//保留3600个数据
        {
            realTimeDataBuffer.pop_back();
            realTimeDataBuffer.push_front(tempdata);
        }
        else
        {
            realTimeDataBuffer.push_front(tempdata);//队列处理
        }
        emit newRealTimeData();//发送数据读取消息
}
/*
 * 电表读数失败
*/
void DataProcessor::ammeterGetDataError()
{
    qDebug()<<"dp am er";
    saveDataTimer->stop();
    monitorTimer->stop();
    AmmeterError = true;
    emit ammeterError();
}

/*
 *UI读取数据函数
 * 返回一个datapoint
 */
DataPoint DataProcessor::getLastData()
{
    return realTimeDataBuffer.first();
}
/*
 * 获得最新的功率数据
*/
datatype DataProcessor::getLastPower()
{
    if(realTimeDataBuffer.isEmpty())
    {
        emit actionError();
        return 0.0;
    }
    return realTimeDataBuffer.first().eps;
}
/*
*向数据库中写入数据
*/
void DataProcessor::saveData()
{
    bool dbreturn;
    if(realTimeDataBuffer.isEmpty())//没有数据存储
    {
        emit actionError();
        return ;
    }
    else
    {
        dbreturn = database->saveData(realTimeDataBuffer.first());//存储当前读取的最新数据
        if(!dbreturn)//数据库存储错误
        {
            saveDataTimer->stop();
            emit dataBaseError();
            return ;
        }
        return ;
    }

}
/*
 * 固定时间间隔进行监控，发送指定的数据
*/
void DataProcessor::monitorAction()
{
    if(realTimeDataBuffer.isEmpty())
    {
        emit actionError();
        return;
    }
    MinPowerDataPoint = getMinPowerDataPoint(MonitorTimeInterval);
    regulator->sendThershold((int)ProporitonLimit,MinPowerDataPoint,realTimeDataBuffer.at(0));
}
/*
*timeLength:计算时间 单位：second
*默认计算10分钟内的平均功率，600个数据
*如果计算时间内的要求数据少于 realdtimedatabuffer 则使用当前记录数据
*/
datatype DataProcessor::getAveragePower(int timeLength)
{
    datatype tempaveragepower = 0.0;
    datatype temptotalpower   = 0.0;
    int   counttime = 0;//计数个数
    int   counter   = 0;//计数

    if(realTimeDataBuffer.isEmpty())
    {
        emit actionError();//没有数据情况下报错
        return 0.0;
    }
    counttime = (timeLength>realTimeDataBuffer.size()?realTimeDataBuffer.size():timeLength);//选取取合理的数据个数
    for(counter = 0;counter >= counttime; counter++)//计算总功率
    {
        temptotalpower += realTimeDataBuffer.at(counter).eps * GetDataTimeInterval;
    }
    tempaveragepower = temptotalpower / (float)counttime;
    TotalPower = temptotalpower;
    return tempaveragepower;
}
/*
*timeLength:计算时间 单位：second
*如果计算时间内的要求数据少于 realdtimedatabuffer 则使用当前记录数据
*/
datatype DataProcessor::getMinPower(int timeLength)
{
    datatype tempminpower = 0.0;
    int   counttime = 0;//计数个数
    int   counter   = 0;//计数

    if(realTimeDataBuffer.isEmpty())//保证有数据
    {
        emit actionError();
        return 0.0;
    }
    counttime = (timeLength>realTimeDataBuffer.size()?realTimeDataBuffer.size():timeLength);//选取取合理的数据个数
    tempminpower = realTimeDataBuffer.first().eps;
    for(counter = 0;counter >= counttime; counter++)//找到最小功率
    {
        tempminpower = (tempminpower<realTimeDataBuffer.at(counter).eps?tempminpower:realTimeDataBuffer.at(counter).eps);//寻找最小功率
    }
    return tempminpower;
}
/*
 * 调节函数开始
*/
void DataProcessor::regulatorAction()
{
    if(realTimeDataBuffer.isEmpty())
    {
        emit actionError();
        return;
    }
    regulator->sendThershold((int)ProporitonLimit,MinPowerDataPoint,realTimeDataBuffer.at(0));
}

void DataProcessor::testAction()
{
    regulator->shutDownHardware();//下位机关机，开始测试
}

void DataProcessor::closeMonitor()
{
    monitorTimer->stop();//关闭监控

}

void DataProcessor::openMonitor()
{
    monitorTimer->start(MonitorTimeInterval*1000);//开启监控

}

void DataProcessor::regulatorConnectError()
{
    RegulatorError = true;
    monitorTimer->stop();
    emit regulatorError();
}


void DataProcessor::testStart()
{
//    monitorTimer->stop();//停止监控计时

    PowerBeforeTest = realTimeDataBuffer.first().eps;
}

void DataProcessor::testCount()
{
    PowerAfterTest = realTimeDataBuffer.first().eps;
    if(PowerBeforeTest != 0.0)
    {
        SavingRate = (PowerBeforeTest - PowerAfterTest) / PowerBeforeTest;
        emit sendTestResult(PowerBeforeTest,PowerAfterTest,SavingRate);
    }
    else
    {
        SavingRate = 0.0;
        emit actionError();
    }
    regulator->startHardware();

}
/*找到当前功率最低点*/
DataPoint DataProcessor::getMinPowerDataPoint(int timeLength)
{
    datatype tempminpower = 0.0;
    DataPoint tempminpowerdatapoint;
    int   counttime = 0;//计数个数
    int   counter   = 0;//计数
    counttime = (timeLength>realTimeDataBuffer.size()?realTimeDataBuffer.size():timeLength);//选取取合理的数据个数
    tempminpower = realTimeDataBuffer.first().eps;
    for(counter = 0;counter >= counttime; counter++)//找到最小功率
    {
        tempminpower = (tempminpower<realTimeDataBuffer.at(counter).eps?tempminpower:realTimeDataBuffer.at(counter).eps);//寻找最小功率
        tempminpowerdatapoint = realTimeDataBuffer.at(counter);//??返回那个电压值
    }
    emit regulatorFinish();//调节完成
    return tempminpowerdatapoint;
}
void DataProcessor::rewritePowerMessage(datatype &ap, datatype &tp, datatype &up, int timeLength)
{
    int counttime = 0;
    int counter = 0;
    ap = getAveragePower(timeLength);
    tp = TotalPower;
    up = 0.0;
    counttime = (timeLength>realTimeDataBuffer.size()?realTimeDataBuffer.size():timeLength);//选取取合理的数据个数
    for(counter = 0;counter >= counttime; counter++)//找到最小功率
    {
        if(realTimeDataBuffer.at(counter).eps>(ap))
        {
            up += (realTimeDataBuffer.at(counter).eps - ap);
        }
    }

}

bool DataProcessor::isAmmeterError()
{
    return AmmeterError;
}

bool DataProcessor::isRegulatorError()
{
    return RegulatorError;
}

/*
 * 历史数据切片
*/
void DataProcessor::dataSlicer(QDateTime begin, QDateTime end, QList<DataPoint> &datapoints, int dataamount)
{
    QList<DataPoint> historyorigindatabuffer;
    database->dataSlicer(begin,end,historyorigindatabuffer);
    int origindataamount = historyorigindatabuffer.size();
    int datainterval = origindataamount / dataamount;
    if(datainterval<1)
    {
        datainterval=1;
    }
    qDebug()<<"begin slicer";
    datapoints.clear();
    for(int i = 0;i < origindataamount;)
    {
        datapoints.push_back(historyorigindatabuffer.at(i));
        i += datainterval;
    }
}
/*
*设置时间间隔 单位：S
* timetype:
* “1”：GetDataTime 采样数据间隔
* ”2“: SaveDataTime 存储数据间隔
* ”3“: MonitorTime  监控时间间隔
*/
void DataProcessor::setTimeInterval(int interval, int timettype)
{
    switch (timettype) {
    case 1:
        GetDataTimeInterval = interval;
        break;
    case 2:
        SaveDataTimeInterval = interval;
        break;
    case 3:
        MonitorTimeInterval = interval;
        break;
    default:
        break;
    }
}
/*
 * 设置限位比例
*/
void DataProcessor::setProportion(float proportion)
{
    if(proportion>0&&proportion<1)
    {
        ProporitonLimit = proportion;
    }
    else
    {
        emit actionError();
    }
}




