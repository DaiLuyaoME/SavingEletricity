/**
 * Ammeter class, responsible for ammeter operation such as data reaing(voltage, current, effective power, etc.)
 *
 * */

#ifndef AMMETER_H
#define AMMETER_H

#include <QObject>
#include <datapoint.h>
class Ammeter : public QObject
{
    Q_OBJECT
public:
    explicit Ammeter(QObject *parent = 0);
    int readLatestData();//return -1 if failed
    DataPoint getData();//get latest data from ammeter

signals:

public slots:


private:
    DataPoint latestData;
};

#endif // AMMETER_H
