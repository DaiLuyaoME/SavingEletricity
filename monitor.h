#ifndef MONITOR_H
#define MONITOR_H

#include <QObject>

class Monitor : public QObject
{
    Q_OBJECT
public:
    explicit Monitor(QObject *parent = 0);

signals:

public slots:
};

#endif // MONITOR_H