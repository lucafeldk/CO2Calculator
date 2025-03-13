#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <iostream>

int main(int argc, char *argv[])
{
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    std::cout << "Current Working Directory: " << QDir::currentPath().toStdString() << std::endl;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
