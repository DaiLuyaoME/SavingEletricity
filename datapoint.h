#ifndef DATAPOINT_H
#define DATAPOINT_H
#define NUM_AMMETER_DATA 22
typedef float datatype;
struct DataPoint
{
    datatype va,vb,vc;  //三相电压（voltage)
    datatype ia,ib,ic;  //三相电流（current）
    datatype epa,epb,epc,eps;   //有功功率（effective power）
    datatype rpa,rpb,rpc,rps;   //无功功率（reactive power）
    datatype apa,apb,apc,aps;   //视在功率（apparent power）
    datatype pfa,pfb,pfc,pfs;   //功率因素（power factor）
};

#endif // DATAPOINT_H
