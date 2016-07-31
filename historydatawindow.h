#ifndef HISTORYDATAWINDOW_H
#define HISTORYDATAWINDOW_H

#include <QWidget>
#include <dataprocessor.h>
namespace Ui {
class HistoryDataWindow;
}

class HistoryDataWindow : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryDataWindow(QWidget *parent = 0);
    ~HistoryDataWindow();

private:
    Ui::HistoryDataWindow *ui;
    DataProcessor *dataPro;
};

#endif // HISTORYDATAWINDOW_H
