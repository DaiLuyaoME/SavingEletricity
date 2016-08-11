#include "dataprocessor.h"

#define REALTIMEDATABUFFERCOUNT 3600//realTimeDataBuffer 数据个数
#define GETDATATIMEDEFAULT      1//默认读取数据间隔 单位：second
#define SAVEDATATIMEDEFALT      10//默认存储数据间隔 单位：second
#define MONITERTIMEDEFALT       600//默认监控计算间隔 单位：second
#define ERROR                   -1//报错回传参数

DataProcessor::DataProcessor(QObject *parent) : QObject(parent)
{
	/*实例初始化*/
    /*！！！有关串口的实例都要注意串口号的占用与释放*/
    database = new Database(this);//数据库实例
	ammeter = new Ammeter(this);//电表实例
	regulator  = new Regulator(this);//下位机实例
	getDataTimer = new QTimer(this);//读取数据实例 1s读取一次数据
	saveDataTimer = new QTimer(this);//存储数据实例 10s存储一次数据
	moniterTimer = new QTimer(this);//电表监控实例 计算功率
    realTimeDataBuffer = new QList<DataPoint>;//数据组实例 ***！！？ 数据个数确定***


    /*参数初始化*/
    OldAveragePower       = 0.0;
    OldTotalPower         = 0.0;
    AveragePower          = 0.0;
    TotalPower            = 0.0;

    GetDataTimeInterval   = GETDATATIMEDEFAULT;
    SaveDataTimeInterval  = SAVEDATATIMEDEFALT;
    MoniterTimeInterval   = MONITERTIMEDEFALT;



	/*1s读取数据*/
	QObject::connect(getDataTimer,SIGNAL(timeout()),this,SLOT(getData()));
    getDataTimer->start(GetDataTimeInterval*1000);//默认1s一次
    /*10S存储数据*/
    QObject::connect(saveDataTimer,SIGNAL(timeout()),this,SLOT(saveData());
    saveDataTimer->start(SaveDataTimeInterval*1000);//默认10s一次
    /*10min监控一次*/
    QObject::connect(moniterTimer,SIGNAL(timeout()),this,SLOT(getAveragePower(MONITERTIME));//计算前10min平均功率
    QObject::connect(moniterTimer,SIGNAL(timeout()),this,SLOT(getMinPower(MONITERTIME));//计算前10min最小功率
    moniterTimer->start(MoniterTimeInterval);//默认600s一次





}

/*
*通过电表ammeter读取数据
*
*/
void DataProcessor::getData()
{
    int Isread;
    DataPoint tempdata;
    Isread = ammeter->readLatestData();
    if(Isread == ERROR)
    {
        emit ammeterError();//发送电表错误消息
    }
    else
    {
        tempdata = ammeter->getData();//读取数据
        if(realTimeDataBuffer.size()>=REALTIMEDATABUFFERCOUNT)//保留3600个数据
        {
            realTimeDataBuffer.pop_front();
            realTimeDataBuffer.push_back(tempdata);
        }
        else
        {
            realTimeDataBufferl.push_back(tempdata);//队列处理
        }
    }

}

/*
*向数据库中写入数据
*/
void DataProcessor::saveData()
{
    bool Isnodata;
    Isnodata = realTimeDataBuffer.isEmpty();
    if(Isnodata)//没有数据存储
    {
        return 0;
    }
    else
    {
        /*!!?如果此处的last()函数返回的是指针,不用加星号“*”。*/
        /*!!!database 类中没有报错机制*/
        database->saveData(realTimeDataBuffer.last());//存储当前读取的最新数据
    }
}

void DataProcessor::startMonitor()
{

}

void DataProcessor::stopMonitor()
{

}

/*
 * 根据绘图所需点的个数来
*/
QList<DataPoint> DataProcessor::dataSlicer(QTime begin, QTime end)
{
    return *(new QList<DataPoint>);
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
        MoniterTimeInterval = interval;
        break;
    default:
        break;
    }
}

/*
*timeLength:计算时间 单位：second
*默认计算10分钟内的平均功率，600个数据
*如果计算时间内的要求数据少于 realdtimedatabuffer 则使用当前记录数据
*/
void DataProcessor::getAveragePower(int timeLength)
{
    float tempaveragepower = 0.0;
    int   counttime = 0;//计数个数
    int   counter   = 0;//计数

    if(realTimeDataBuffer.isEmpty())
    {
        emit actionError();
    }
    counttime = (timeLength>realTimeDataBuffer.size()?realTimeDataBuffer.size():timeLength);//选取取合理的数据个数

    for(i = counttime;i <= 0; i--)//计算总功率
    {
        TotalPower += realTimeDataBuffer.at(i);
    }
    AveragePower = TotalPower / counttime;//计算平均功率
    OldAveragePower = AveragePower;
    OldTotalPower = TotalPower;
    TotalPower = 0;//总功率计数清零
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
    }
    counttime = (timeLength>realTimeDataBuffer.size()?realTimeDataBuffer.size():timeLength);//选取取合理的数据个数

    tempminpower = realTimeDataBuffer.last();
    for(i = counttime;i <= 0; i--)//找到最小功率
    {
        tempminpower = (tempminpower<realTimeDataBuffer.at(i)?tempminpower:realTimeDataBuffer.at(i));//寻找最小功率
    }
    MinMomentPower = tempminpower;
    return tempminpower;
}
