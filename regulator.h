/**
 * Regulator class，namely the lower computer, responsible for the communication with regulator(lower computer)
 * */
#ifndef REGULATOR_H
#define REGULATOR_H

#include <QObject>
#include <datapoint.h>
class Regulator : public QObject
{
    Q_OBJECT
public:
    explicit Regulator(QObject *parent = 0);
    void sendVotageAtMinPower(datatype voltage);
    void beginRegulate();
    void dataParser();//parse the data from the regulator
    void closePort();
    void openPort();
    void ack();

signals:
    void regulationOver();//signal that regulation is over
    void regulatorError();

public slots:
};

#endif // REGULATOR_H
