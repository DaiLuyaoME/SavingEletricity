#include "database.h"


Database::Database(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    qDebug()<<"construct database";
    db.setDatabaseName(QString("data.db"));
    qDebug()<<db.open();
    qDebug()<<db.tables().length();
    if (db.tables().length()==0)
        qDebug()<<createTable();

}
void Database::close()
{
    db.close();
}

bool Database::createTable()
{
    QSqlQuery query;
    return query.exec("create table records (id int primary key, "
                   "date INTEGER, va FLOAT,vb FLOAT,vc FLOAT,"
               "ia FLOAT,ib FLOAT,ic FLOAT,"
               "epa FLOAT,epb FLOAT, epc FLOAT, eps FLOAT,"
               "rpa FLOAT, rpb FLOAT, rpc FLOAT, rps FLOAT,"
               "apa FLOAT, apb FLOAT, apc FLOAT, aps FLOAT,"
               "pfa FLOAT, pfb FLPAT, pfc FLOAT, pfs FLOAT)");
}

bool Database::saveData(DataPoint dataToSave)
{
    QSqlQuery query;
    uint datetime=QDateTime::currentDateTime().toTime_t();
    qDebug()<<"the datetime of saving data is "<<QDateTime::currentDateTime();
    qDebug()<<datetime;
    query.prepare("INSERT INTO records (date, va, vb,vc,"
                  "ia,ib,ic,"
                  "epa,epb,epc,eps,"
                  "rpa,rpb,rpc,rps,"
                  "apa,apb,apc,aps,"
                  "pfa,pfb,pfc,pfs) "
                  "VALUES (:date, :va, :vb,:vc,"
                  ":ia,:ib,:ic,"
                  ":epa,:epb,:epc,:eps,"
                  ":rpa,:rpb,:rpc,:rps,"
                  ":apa,:apb,:apc,:aps,"
                  ":pfa,:pfb,:pfc,:pfs)");
    query.bindValue(":date", datetime);
    query.bindValue(":va",dataToSave.va);
    query.bindValue(":vb",dataToSave.vb);
    query.bindValue(":vc",dataToSave.vc);
    query.bindValue(":ia",dataToSave.ia);
    query.bindValue(":ib",dataToSave.ib);
    query.bindValue(":ic",dataToSave.ic);
    query.bindValue(":epa",dataToSave.epa);
    query.bindValue(":epb",dataToSave.epb);
    query.bindValue(":epc",dataToSave.epc);
    query.bindValue(":eps",dataToSave.eps);
    query.bindValue(":rpa",dataToSave.rpa);
    query.bindValue(":rpb",dataToSave.rpb);
    query.bindValue(":rpc",dataToSave.rpc);
    query.bindValue(":rps",dataToSave.rps);

    query.bindValue(":apa",dataToSave.apa);
    query.bindValue(":apb",dataToSave.apb);
    query.bindValue(":apc",dataToSave.apc);
    query.bindValue(":aps",dataToSave.aps);

    query.bindValue(":pfa",dataToSave.pfa);
    query.bindValue(":pfb",dataToSave.pfb);
    query.bindValue(":pfc",dataToSave.pfc);
    query.bindValue(":pfs",dataToSave.pfs);

    return query.exec();
}

void Database::dataSlicer(QDateTime begin, QDateTime end, QList<DataPoint> &slicedData)
{
    QSqlQuery query;
    uint startdatetime=begin.toTime_t();
    uint enddatetime=end.toTime_t();
    QString q=QString("SELECT * From records where date>=%1 and date<=%2").arg(startdatetime).arg(enddatetime);
    query.exec(q);
    DataPoint newpoint;
    while(query.next())
    {
//        qDebug()<<"one record";
        // DataPoint* newpoint=new DataPoint();
        newpoint.va=query.value("va").toFloat();
        newpoint.vb=query.value("vb").toFloat();
        newpoint.vc=query.value("vc").toFloat();
        newpoint.ia=query.value("ia").toFloat();
        newpoint.ib=query.value("ib").toFloat();
        newpoint.ic=query.value("ic").toFloat();
        newpoint.epa=query.value("epa").toFloat();
        newpoint.epb=query.value("epb").toFloat();
        newpoint.epc=query.value("epc").toFloat();
        newpoint.eps=query.value("eps").toFloat();
        newpoint.rpa=query.value("rpa").toFloat();
        newpoint.rpb=query.value("rpb").toFloat();
        newpoint.rpc=query.value("rpc").toFloat();
        newpoint.rps=query.value("rps").toFloat();
        newpoint.apa=query.value("apa").toFloat();
        newpoint.apb=query.value("apb").toFloat();
        newpoint.apc=query.value("apc").toFloat();
        newpoint.aps=query.value("aps").toFloat();
        newpoint.pfa=query.value("pfa").toFloat();
        newpoint.pfb=query.value("pfb").toFloat();
        newpoint.pfc=query.value("pfc").toFloat();
        newpoint.pfs=query.value("pfs").toFloat();
        // slicedData.append(*newpoint);
        slicedData.append(newpoint);
    }
    qDebug()<<"database ok";
}
