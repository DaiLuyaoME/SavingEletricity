#include "historydatawindow.h"
#include "ui_historydatawindow.h"

HistoryDataWindow::HistoryDataWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryDataWindow)
{
    ui->setupUi(this);
}

HistoryDataWindow::~HistoryDataWindow()
{
    delete ui;
}
