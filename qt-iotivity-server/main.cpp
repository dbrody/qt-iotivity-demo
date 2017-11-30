#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow* w1 = new MainWindow();
    w1->show();
    w1->resize(600, 300);
    w1->move(340, 20);
    return a.exec();
}
