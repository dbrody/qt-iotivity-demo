#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    server(new IoTServer)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete server;
}
