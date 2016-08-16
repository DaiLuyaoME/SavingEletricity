#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    historyWin.setParentWin(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    realTimeWin.show();
    this->hide();
}

void MainWindow::on_pushButton_3_clicked()
{
    testWin.show();
    this->hide();
}

void MainWindow::on_pushButton_2_clicked()
{
    historyWin.show();
    this->hide();
}
