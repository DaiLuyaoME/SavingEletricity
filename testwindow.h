#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QWidget>
#include <dataprocessor.h>
#include <QStandardItemModel>
#include <QMessageBox>
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
    explicit TestWindow(QWidget *parent = 0,DataProcessor *processor=0);
    ~TestWindow();
private:
    Ui::TestWindow *ui;
    DataProcessor *dataPro;
    QStandardItemModel testModel;
    QList<TestRecord> testRecords;

private slots:
    void testEnd();
    void on_beginTest_clicked();
    void on_testEnd_clicked();
    void on_clearTestRecords_clicked();
    void on_returnBtn_clicked();
    void on_calculateRatioButton_clicked();

public slots:
    void getResult(datatype powerBefore,datatype powerAfter,float ratio);

signals:
    void testButtonClicked();
};

#endif // TESTWINDOW_H
