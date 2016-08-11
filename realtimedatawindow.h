#ifndef REALTIMEDATAWINDOW_H
#define REALTIMEDATAWINDOW_H

#include <QWidget>
#include <dataprocessor.h>
#include "qcustomplot.h"

#define VMODE 0
#define IMODE 1
#define EPMODE 2
#define RPMODE 3
#define APMODE 4
#define PFMODE 5

namespace Ui {
class RealTimeDataWindow;
}

class RealTimeDataWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RealTimeDataWindow(QWidget *parent = 0);
    ~RealTimeDataWindow();

private slots:

    void on_vRadioBtn_toggled(bool checked);
    void on_iRadioBtn_toggled(bool checked);
    void on_epRadioBtn_toggled(bool checked);
    void on_fpRadioBtn_toggled(bool checked);
    void on_apRadioBtn_toggled(bool checked);
    void on_pfRadioBtn_toggled(bool checked);
    void on_horizontalSlider_sliderReleased();

private:
    int mode,range,removeIdx,addIdx;
    Ui::RealTimeDataWindow *ui;
    DataProcessor *dataPro;
    QList<DataPoint> datapoints;
    QTimer updateTimer;

    void replot(int mode,int range);
    void trimdata(int range);
    void addDataToPlot();
};

#endif // REALTIMEDATAWINDOW_H
