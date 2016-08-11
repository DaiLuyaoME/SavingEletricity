#include "dataprocessor.h"

DataProcessor::DataProcessor(QObject *parent) : QObject(parent)
{

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
    return *(new QList<DataPoint>);
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
