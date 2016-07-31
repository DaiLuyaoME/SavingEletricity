#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QWidget>
#include <dataprocessor.h>
namespace Ui {
class TestWindow;
}

class TestWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TestWindow(QWidget *parent = 0);
    ~TestWindow();

private:
    Ui::TestWindow *ui;
    DataProcessor *dataPro;
};

#endif // TESTWINDOW_H
