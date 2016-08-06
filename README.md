# SavingEletricity
# UI

## MainWindow

- 四个功能按钮
- 关机

## RealTimeDataWindow

- 用户选择看哪种数据
- 量程档位：1分钟，十分钟，一小时
- slot add data

## HistoryDataWindow

- 用户选择那种数据
- 用户可输入开始和结束（年月日，时），数据点数确定

## TestWindow
- //下位机实例
- //若测试则先关闭dataprocessor下位机实例和monitor timer
- 


- 手动测试：清除数据
- manualregulator();
- 自动测试

## ConfigWindow


# Dataprocessor
private variables:
- QArraylist<datapoint> realtimeData:1point/1s,length=3600
- Timer getdatatimer 1s
- Timer savedatatimer 10s
- TImer monitortimer 10min
- 电表类实例
- 下位机
- Database 

public functions:
- QArraylist<datapoint> dataSlicer(time start,time end)
- slot for getdata // read data from 电表
- slot for savedata // write data to database
- settimerinterval() *2
- getAveragePower(int timelength unit: second)
- getMinimalPower(int timelength unit: second)
- slot for monitor //监视要不要调
- slot for manual regulation
- slot for auto regulation
- 
- stopMonitor()
- beginMonitor()


Signals：
- 电表 error
- regulator error
- database error

# Regulator
要求： 多次尝试

- beginRegulateWithTarget(int target): new timer to monitor regulation timeout
- beginRegulate(): new timer to monitor regulation timeout
- dataParser(): emit a signal when retulation ends
- closePort()
- openPort()
- ack()

# 电表
datapoint latestdata
- datapoint getData()
- int readLatestData(); -1 则通信失败，0则成功
- int ack()

# Database
dataSlicer(time start, time end, QArrayList<datapoint>) from database get data and transform them into datapoint
saveData(datapoint)

# DataStructure.h
structure Datapoint
{
    float va,vb,vc;
    float ia,ib,ic;
    float epa,epb,epc,eps;
    float rpa,rpb,rpc,rps;
    float apa,apb,apc,aps;
    float pfa,pfb,pfc,pfs;
}

UI，database：wyh
dataprocessor:LY
电表下位机：DLY



2016/08/04
# 程序启动流程
1、main
2、UI实例
3、UI实例中创建dataprocessor实例
4、dataprocessor实例中创建database实例 ammete实例 regulator实例

# 问题与缺陷：
1、有些类没有析构函数
2、database中没有报错机制
3、settimerinterval() 这个函数是用来设置什么时间间隔的？
4、计算Pa,Sh,Pt（见项目书）？？



2016/08/05
# 发现问题
1、计算平局功率、总做功、最小功率时，要考虑机器启动时间较短情况下，数据量不满足时间要求的情况。
我的处理：
增加 actionError() 这个signal应对操作不当的情况。

  