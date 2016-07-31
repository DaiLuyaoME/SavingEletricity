#include "realtimedatawindow.h"
#include "ui_realtimedatawindow.h"

RealTimeDataWindow::RealTimeDataWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RealTimeDataWindow)
{
    ui->setupUi(this);
}

RealTimeDataWindow::~RealTimeDataWindow()
{
    delete ui;
}
