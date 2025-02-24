#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QCheckBox>
#include <QListWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringList generationList = {
        "Generation", "Biomass", "Fossil Brown coal", "Fossil Coal-derived gas",
        "Fossil Gas", "Fossil Hard Coal", "Fossil Oil", "Fossil Oil Shale", "Fossil Peat",
        "Geothermal", "Hydro Pumped Storage", "Hydro Run-of-river and poundage",
        "Hydro Water Reservoir", "Marine", "Nuclear", "Other renewable", "Solar", "Waste",
        "Wind Offshore", "Wind Onshore", "Other", "Energy storage"
    };

    QStringList requestValues = {
        "Generated Power in Megawatt",
        "Emitted CO2 Emissions in tons"
    };

    setupCheckBoxList(ui->listGenerationType,generationList);
    setupCheckBoxList(ui->listRequestValues,requestValues);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupCheckBoxList(QListWidget* listWidget, QStringList items){
    if (!listWidget) return;

    listWidget->clear();

    for (const QString &itemText : items) {
        QListWidgetItem *item = new QListWidgetItem(listWidget);
        QCheckBox *checkBox = new QCheckBox(itemText);
        listWidget->setItemWidget(item, checkBox);
    }

}

void MainWindow::on_applySettingsBtn_clicked()
{
    // Event handling for Apply Settings Button clickend

    // 1. Get_Requested Data
    // 2. Edit Plot Graph
    // 3. Turn Button inactive

    //Note: maybe make this a dynamic change when single parameters are edited

}


void MainWindow::on_listGenerationType_itemClicked(QListWidgetItem *item)
{
    // List of Generation Type
    // Event handling for clicking checkbox item from this List Widget

    // 1. If new changes occur, turn apply settings button active
    // 2. change request data accordingly

}


void MainWindow::on_listRequestValues_itemClicked(QListWidgetItem *item)
{
    // List of Requested Values
    // Event handling for clicking checkbox item from this List Widget

    // 1. If new changes occur, turn apply settings button active
    // 2. change request data accordingly
}


void MainWindow::on_fontComboBox_currentTextChanged(const QString &arg1)
{
    // Event handling for country change
}


void MainWindow::on_startTime_dateTimeChanged(const QDateTime &dateTime)
{
    // Event handling for changing the starttime
    // Starttime can't exceed the endtime
}


void MainWindow::on_endTime_dateTimeChanged(const QDateTime &dateTime)
{
    // Event handling for changing the endtime
    // Endtime cannot fall below the startime
}


void MainWindow::on_screenshotBtn_clicked()
{
    // Event handling for screenshot button
    // Saves plot to img file
}


void MainWindow::on_downloadButton_clicked()
{
    // Event handling for download button
    // downloads requested data to .csv
}

