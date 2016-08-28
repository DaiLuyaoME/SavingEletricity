 /**
 * Database class, responsible for data operation such as storage, reading and slicing
 *
*/

#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <datapoint.h>
#include <QTime>
#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>
#include <QVariant>
class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = 0);
    bool createTable();
    void close();
private:
    QSqlDatabase db;
//    const char bindNames[23][7]={":date",":va",":vb",":vc",
//                                  ":ia",":ib",":ic",
//                                  ":epa",":epb",":epc",":eps",
//                                  ":rpa",":rpb",":rpc",":rps",
//                                  ":apa",":apb",":apc",":aps",
//                                  ":pfa",":pfb",":pfc",":pfs"};

signals:

public slots:
    bool saveData(DataPoint dataToSave);
    void dataSlicer(QDateTime begin, QDateTime end, QList<DataPoint> &slicedData);
};

#endif // DATABASE_H
