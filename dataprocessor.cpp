#include "dataprocessor.h"

DataProcessor::DataProcessor(QObject *parent) : QObject(parent)
{
	/*实例初始化*/
	/*有关串口的实例都要注意串口号的占用与释放*/
	ammeter = new Ammeter(this);//电表实例
	regulator  = new Regulator(this);//下位机实例
	getDataTimer = new QTimer(this);//读取数据实例 1s读取一次数据
	saveDataTimer = new QTimer(this);//存储数据实例 10s存储一次数据
	moniterTimer = new QTimer(this);//电表监控实例 计算功率

	realTimeDataBuffer???
	/*1s读取数据*/
	QObject::connect(getDataTimer,SIGNAL(timeout()),this,SLOT(getData()));
	getDataTimer->start(1000);//1s一次

	/*存储数据*/


}

void DataProcessor::getData()
{

}

void DataProcessor::saveData()
{

}

void DataProcessor::startMonitor()
{

}

void DataProcessor::stopMonitor()
{

}

QList<DataPoint> DataProcessor::dataSlicer(QTime begin, QTime end)
{

}

void DataProcessor::setTimeInterval()
{

}

void DataProcessor::getAveragePower(int timeLength)
{

}

void DataProcessor::getMinPower(int timeLength)
{

}
