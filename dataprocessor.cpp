#include "dataprocessor.h"
#include "math.h"
#define REALTIMEDATABUFFERCOUNT 3600//realTimeDataBuffer 数据个数
#define GETDATATIMEDEFAULT      1//默认读取数据间隔 单位：second
#define SAVEDATATIMEDEFALT      10//默认存储数据间隔 单位：second
#define MONITERTIMEDEFALT       600//默认监控计算间隔 单位：second
#define ERROR                   -1//报错回传参数

DataProcessor::DataProcessor(QObject *parent) : QObject(parent)
{
	/*实例初始化*/
    database = new Database(this);//数据库实例
	ammeter = new Ammeter(this);//电表实例
	regulator  = new Regulator(this);//下位机实例
    getDataTimer = new QTimer(this);//读取数据计时器
    saveDataTimer = new QTimer(this);//存储数据计时器
    monitorTimer = new QTimer(this);//电表监控计时器
    RegulatorTime = new QTime();//下位机计时
    /*参数初始化*/
    AveragePower          = 0.0;
    TotalPower            = 0.0;

    GetDataTimeInterval   = GETDATATIMEDEFAULT;
    SaveDataTimeInterval  = SAVEDATATIMEDEFALT;
    MonitorTimeInterval   = MONITERTIMEDEFALT;
    /*读取数据*/
    QObject::connect(ammeter,SIGNAL(getDataOver()),this,SLOT(getData()));
    QObject::connect(ammeter,SIGNAL(ammeterError()),this,SLOT(ammeterGetDataError()));
    /*存储数据*/
    QObject::connect(saveDataTimer,SIGNAL(timeout()),this,SLOT(saveData()));
    saveDataTimer->start(SaveDataTimeInterval*1000);//默认10s一次
    /*监控一次*/
    QObject::connect(monitorTimer,SIGNAL(timeout()),this,SLOT(monitorAction()));
    monitorTimer->start(MonitorTimeInterval);//默认600s一次
   /*regulator 回传电压值*/
    QObject::connect(regulator,SIGNAL(regulationBegun()),this,SLOT(regulatorStart()));
    QObject::connect(regulator,SIGNAL(regulationOver()),this,SLOT(regulatorCount()));
    QObject::connect(regulator,SIGNAL(regulatorError(RegulatorInstructionType)),this,SLOT(regulatorActionError()));



}
DataProcessor::~DataProcessor()
{

}
/*
*通过电表ammeter读取数据
*/
void DataProcessor::getData()
{
    DataPoint tempdata;
        tempdata = ammeter->getData();//读取数据
        if(realTimeDataBuffer.size()>=REALTIMEDATABUFFERCOUNT)//保留3600个数据
        {
            realTimeDataBuffer.pop_front();
            realTimeDataBuffer.push_back(tempdata);
        }
        else
        {
            realTimeDataBuffer.push_back(tempdata);//队列处理
        }
        emit newRealTimeData();//发送数据读取消息
}
/*
 * 电表读数失败
*/
void DataProcessor::ammeterGetDataError()
{
    saveDataTimer->stop();
    monitorTimer->stop();
    emit ammeterError();
}

/*
 *UI读取数据函数
 * 返回一个datapoint
 */
DataPoint DataProcessor::getLastData()
{
    return realTimeDataBuffer.last();
}
/*
 * 获得最新的功率数据
*/
float DataProcessor::getLastPower()
{
    return realTimeDataBuffer.last().eps;
}
/*
*向数据库中写入数据
*/
int DataProcessor::saveData()
{
    bool Isnodata;
    bool dbreturn;
    Isnodata = realTimeDataBuffer.isEmpty();
    if(Isnodata)//没有数据存储
    {
        emit actionError();
        return -1;
    }
    else
    {
        dbreturn = database->saveData(realTimeDataBuffer.last());//存储当前读取的最新数据
        if(dbreturn == ERROR)//数据库存储错误
        {
            saveDataTimer->stop();
            emit dataBaseError();
            return -1;
        }
//          database->saveData(realTimeDataBuffer.last());//存储当前读取的最新数据
        return 0;
    }

}
/*
 * 固定时间间隔进行监控，满足一定条件触regulator调节
*/
void DataProcessor::monitorAction()
{
    float templastpower = realTimeDataBuffer.last().eps;//计算有功功率
    float temppowerporprotion = 0.0;
    AveragePower = getAveragePower(MonitorTimeInterval);
    temppowerporprotion = AveragePower * ProporitonLimit;
    if(templastpower>(AveragePower+temppowerporprotion)||templastpower<(AveragePower-temppowerporprotion))
    {
        regulatorAction();
    }
}
/*
*timeLength:计算时间 单位：second
*默认计算10分钟内的平均功率，600个数据
*如果计算时间内的要求数据少于 realdtimedatabuffer 则使用当前记录数据
*/
float DataProcessor::getAveragePower(int timeLength)
{
    float tempaveragepower = 0.0;
    float temptotalpower   = 0.0;
    int   counttime = 0;//计数个数
    int   counter   = 0;//计数

    if(realTimeDataBuffer.isEmpty())
    {
        emit actionError();//没有数据情况下报错
        return 0.0;
    }
    counttime = (timeLength>realTimeDataBuffer.size()?realTimeDataBuffer.size():timeLength);//选取取合理的数据个数
    for(counter = (realTimeDataBuffer.size()-1);counter <= (realTimeDataBuffer.size()-counttime); counter--)//计算总功率
    {
        temptotalpower += realTimeDataBuffer.at(counter).eps;
    }
    tempaveragepower = temptotalpower / (float)counttime;
    return tempaveragepower;
}
/*
*timeLength:计算时间 单位：second
*如果计算时间内的要求数据少于 realdtimedatabuffer 则使用当前记录数据
*/
float DataProcessor::getMinPower(int timeLength)
{
    float tempminpower = 0.0;
    int   counttime = 0;//计数个数
    int   counter   = 0;//计数

    if(realTimeDataBuffer.isEmpty())//操作限定在读取数据之后
    {
        emit actionError();
        return 0.0;
    }
    counttime = (timeLength>realTimeDataBuffer.size()?realTimeDataBuffer.size():timeLength);//选取取合理的数据个数
    tempminpower = realTimeDataBuffer.last().eps;
    for(counter = (realTimeDataBuffer.size()-1);counter <= (realTimeDataBuffer.size()-counttime); counter--)//找到最小功率
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
    regulator->beginRegulate();
}
/*
 * 调节函数回传开始动作 开始计时
*/
void DataProcessor::regulatorStart()
{
    RegulatorTime->restart();
}
/*
 * 下位机错误信号
*/
void DataProcessor::regulatorActionError()
{
    RegulatorTime->elapsed();
    emit regulatorError();
}
/*
 * 调节函数结束
 * 给下位机返回调控时间内最低功率点的数据点
 */
void DataProcessor::regulatorCount()
{
    int counttime = RegulatorTime->elapsed() / 1000;//计算间隔时间
    DataPoint tempminpowerdatapoint = getMinPowerDataPoint(counttime);
    regulator->sendVoltageAtMinPower(tempminpowerdatapoint);
}
/*找到当前功率最低点*/
DataPoint DataProcessor::getMinPowerDataPoint(int timeLength)
{
    float tempminpower = 0.0;
    DataPoint tempminpowerdatapoint;
    int   counttime = 0;//计数个数
    int   counter   = 0;//计数
    counttime = (timeLength>realTimeDataBuffer.size()?realTimeDataBuffer.size():timeLength);//选取取合理的数据个数
    tempminpower = realTimeDataBuffer.last().eps;
    for(counter = (realTimeDataBuffer.size()-1);counter <= (realTimeDataBuffer.size()-counttime); counter--)//找到最小功率
    {
        tempminpower = (tempminpower<realTimeDataBuffer.at(counter).eps?tempminpower:realTimeDataBuffer.at(counter).eps);//寻找最小功率
        tempminpowerdatapoint = realTimeDataBuffer.at(counter);//??返回那个电压值
    }
    float MP = realTimeDataBuffer.at(realTimeDataBuffer.size()-counttime).eps;
    float MS = realTimeDataBuffer.last().eps;
    SavingRate = (MP-MS)/MP;
    emit regulatorFinish();//调节完成
    return tempminpowerdatapoint;
}
float DataProcessor::getSavingRate()
{
    return SavingRate;
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
    for(int i = 0;i <= origindataamount;)
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
* ”3“: MoniterTime  监控时间间隔
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




