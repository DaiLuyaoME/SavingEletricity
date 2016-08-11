#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QWidget>
#include <dataprocessor.h>
#include <QStandardItemModel>
struct TestRecord {
    float before,after,ratio;
};
namespace Ui {
class TestWindow;
}

class TestWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TestWindow(QWidget *parent = 0);
    ~TestWindow();
    void setDataprocessor(DataProcessor &dprocessor);

private:
    Ui::TestWindow *ui;
    DataProcessor dataProcessor;
    QStandardItemModel testModel;
    QList<TestRecord> testRecords;

private slots:
    void testEnd();
    void on_beginTest_clicked();
    void on_testEnd_clicked();
    void on_clearTestRecords_clicked();
};

#endif // TESTWINDOW_H
