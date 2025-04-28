#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QMessageBox>
#include <QDir>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

        // Konfigurationsdatei prüfen
    if (!QFile::exists("config.json")) {
         QMessageBox::critical(nullptr, "Fehlende Konfiguration",
                                "Die Datei 'config.json' wurde nicht gefunden.\n"
                                "Bitte kopiere 'config.json.example' zu 'config.json' und trage deinen API-Key ein.");
        return 1;
        }

    QDir::setCurrent(QCoreApplication::applicationDirPath());
    std::cout << "Current Working Directory: " << QDir::currentPath().toStdString() << std::endl;
    MainWindow w;
    w.show();
    return a.exec();
}
