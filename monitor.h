#ifndef MONITOR_H
#define MONITOR_H

#include <QObject>
#include <datapoint.h>
#include <database.h>
#include <regulator.h>
#include <ammeter.h>
#include <QTimer>
#include <QList>
#include <QTime>

class Monitor : public QObject
{
    Q_OBJECT
public:
    explicit Monitor(QObject *parent = 0);


signals:

public slots:
};

#endif // MONITOR_H
