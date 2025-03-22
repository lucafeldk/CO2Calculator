#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    std::cout << "Current Working Directory: " << QDir::currentPath().toStdString() << std::endl;
    MainWindow w;
    w.show();
    return a.exec();
}
