#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    client(new IoTClient)
{
    ui->setupUi(this);

    QObject::connect(client, SIGNAL(resourceDiscovered()), this, SLOT(resourceDiscovered()));

    client->findResource();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete client;
}

void MainWindow::resourceDiscovered()
{
    qDebug() << "Discovered Resources: ";
    if(client->getSwitchSensor()){
        qDebug() << "Switch: " << QString::fromStdString(client->getSwitchSensor()->getHandle()->uri());
        client->getSwitchSensor()->put(true);
    }
}
