#ifndef HISTORYDATAWINDOW_H
#define HISTORYDATAWINDOW_H

#include <QWidget>
#include <dataprocessor.h>
#include "qcustomplot.h"

#define NONMODE -1
#define VMODE 0
#define IMODE 1
#define EPMODE 2
#define RPMODE 3
#define APMODE 4
#define PFMODE 5

namespace Ui {
class HistoryDataWindow;
}

class HistoryDataWindow : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryDataWindow(QWidget *parent = 0, DataProcessor *processor=0);
    ~HistoryDataWindow();


private slots:
    void on_vRadioBtn_toggled(bool checked);
    void on_iRadioBtn_toggled(bool checked);
    void on_epRadioBtn_toggled(bool checked);
    void on_fpRadioBtn_toggled(bool checked);
    void on_apRadioBtn_toggled(bool checked);
    void on_pfRadioBtn_toggled(bool checked);
    void on_setRangeBtn_clicked();
    void on_returnBtn_clicked();

    void on_sincBtn_2_clicked();

    void on_sdecBtn_2_clicked();

    void on_eincBtn_clicked();

    void on_edecBtn_clicked();

//    void on_vRadioBtn_clicked();

//    void on_iRadioBtn_clicked();

private:
    DataProcessor *dataPro;
    Ui::HistoryDataWindow *ui;
    QCustomPlot plots[4];
    QList<DataPoint> datapoints;
    int mode,range;

    void replot(int mode,int range);

};

#endif // HISTORYDATAWINDOW_H
