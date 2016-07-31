/**
 * Database class, responsible for data operation such as storage, reading and slicing
 * */
#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <datapoint.h>
#include <QTime>
#include <QList>
class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = 0);

signals:

public slots:
    void saveData(DataPoint dataToSave);
    void dataSlicer(QTime begin,QTime end,QList<DataPoint> slicedData);
};

#endif // DATABASE_H
