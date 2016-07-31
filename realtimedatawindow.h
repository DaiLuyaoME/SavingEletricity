#ifndef REALTIMEDATAWINDOW_H
#define REALTIMEDATAWINDOW_H

#include <QWidget>
#include <dataprocessor.h>
namespace Ui {
class RealTimeDataWindow;
}

class RealTimeDataWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RealTimeDataWindow(QWidget *parent = 0);
    ~RealTimeDataWindow();

private:
    Ui::RealTimeDataWindow *ui;
    DataProcessor *dataPro;
};

#endif // REALTIMEDATAWINDOW_H
