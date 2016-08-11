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
- 下位机实例
- 若测试则先关闭dataprocessor下位机实例和monitor timer


- 手动测试：清除数据
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
- stopMonitor()
- beginMonitor()
- 

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
- dataSlicer(time start, time end, QArrayList<datapoint>) from database get data and transform them into datapoint
- saveData(datapoint)

# DataStructure.h
''C
structure Datapoint
{
    float va,vb,vc;
    float ia,ib,ic;
    float epa,epb,epc,eps;
    float rpa,rpb,rpc,rps;
    float apa,apb,apc,aps;
    float pfa,pfb,pfc,pfs;
}
''
UI，database：wyh
dataprocessor:LY
电表下位机：DLY
