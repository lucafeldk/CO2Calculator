#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QCheckBox>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QWidget>
#include "DataProvider.h"
#include <vector>
#include "qcustomplot.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Ensure startTime and endTime step by 15-minute intervals
    connect(ui->startTime, &QDateTimeEdit::editingFinished, this, [=]() {
        roundMinutes(ui->startTime);
    });
    connect(ui->endTime, &QDateTimeEdit::editingFinished, this, [=]() {
        roundMinutes(ui->endTime);
    });

    // Ensure stepping moves by 15 minutes
    connect(ui->startTime, &QAbstractSpinBox::stepBy, this, [=](int steps) {
        stepByMinutes(ui->startTime, steps);
    });
    connect(ui->endTime, &QAbstractSpinBox::stepBy, this, [=](int steps) {
        stepByMinutes(ui->endTime, steps);
    });

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

    QStringList inDomain = {
        "Denmark",
        "Germany"
    };


    setupCheckBoxList(ui->listGenerationType,generationList);
    setupCheckBoxList(ui->listRequestValues,requestValues);
    QDateTime minDateTime(QDate(2015, 1, 1), QTime(0, 0));
    QDateTime defaultStartTime(QDate(2020,1,1), QTime(0,0));
    ui->startTime->setMinimumDateTime(minDateTime);
    ui->startTime->setDateTime(defaultStartTime);
    ui->endTime->setDateTime(defaultStartTime.addMonths(1));


    ui ->chooseCountryBox->clear();
    for (const QString &itemText : inDomain){
        ui ->chooseCountryBox->addItem(itemText);
    }
    ui->chooseCountryBox->setCurrentText("Germany"); // set default value

    // Neues QCustomPlot-Objekt erstellen
    QCustomPlot *customPlot = new QCustomPlot(this);

    // QCustomPlot in das vorhandene plotWidget integrieren
    customPlot->setParent(ui->plotWidget);
    customPlot->setGeometry(ui->plotWidget->rect()); // Größe anpassen

    // Einfaches Plot-Beispiel
    QVector<double> x = {1,2,3,4,5,6};
    QVector<double> y = {10,3,4,5,4,20};


    QCPGraph *graph1 = customPlot->addGraph();
    QCPBars *bars1 = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    bars1->setData(x, y);
    customPlot->xAxis->setLabel("Datetime");
    customPlot->yAxis->setLabel("Generated Power in MW");

    customPlot->rescaleAxes(); // Achsen anpassen
    customPlot->replot(); // Plot zeichnen
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupCheckBoxList(QListWidget* listWidget, QStringList items){
    if (!listWidget) return;

    listWidget->clear();

    // create listWidget with checkbox
    for (const QString &itemText : items){
        QListWidgetItem* item = new QListWidgetItem(itemText, listWidget);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // add new flag so item is checkable
        item->setCheckState(Qt::Unchecked); //set default check state
    }
}

QStringList MainWindow::getCheckedItems(QListWidget* listWidget){
    QStringList checkedItems;

    //iterate through items of listwidget and save checked items
    for (int i = 0; i < listWidget->count(); ++i){
        QListWidgetItem* item = listWidget->item(i);

        if(item->checkState() == Qt::Checked){
            checkedItems.append(item->text());
        }
    }

    return checkedItems;
}

void MainWindow::on_applySettingsBtn_clicked()
{
    // Event handling for Apply Settings Button clickend

    // 1. Get_Requested Data
    // 2. Edit Plot Graph
    // 3. Turn Button inactive

    //Note: maybe make this a dynamic change when single parameters are edited
    //DataProvider DataProvider1;
    QStringList selectedPsr = getCheckedItems(ui->listGenerationType);
    std::cout << selectedPsr.size() << std::endl;
    QStringList selectetVal = getCheckedItems((ui->listGenerationType));
    std::cout << selectedPsr.size() << std::endl;
    QString selectedDomain = ui->chooseCountryBox->currentText();
    std::cout << selectedDomain.toStdString() << std::endl;
    QString selectedStart = ui->startTime->dateTime().toString("yyyyMMddHHmm");
    std::cout << selectedStart.toStdString() << std::endl;
    QString selectedEnd = ui->endTime->dateTime().toString("yyyyMMddHHmm");
    std::cout << selectedEnd.toStdString() << std::endl;
    DataProvider DataProvider1;

    std::vector <double> requestData = DataProvider1.get_data("202401282200","202401282230","Fossil Brown coal", "Generation_MW","Germany","Actual generation per type", "Realised");
    for (double& i :requestData){
        std::cout << std::to_string(i) << std::endl;
    }
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


void MainWindow::roundMinutes(QDateTimeEdit *dateTimeEdit) {
    QTime currentTime = dateTimeEdit->time();
    int minutes = currentTime.minute();

    // Nearest 15-minute rounding
    int roundedMinutes = (minutes + 7) / 15 * 15;
    if (roundedMinutes >= 60) {
        dateTimeEdit->setTime(QTime(currentTime.hour() + 1, 0));
    } else {
        dateTimeEdit->setTime(QTime(currentTime.hour(), roundedMinutes));
    }
}

void MainWindow::stepByMinutes(QDateTimeEdit *dateTimeEdit, int steps) {
    QTime currentTime = dateTimeEdit->time();
    int minutes = currentTime.minute();

    // List of valid minute values
    QList<int> validMinutes = {0, 15, 30, 45};

    // Find the current minute position in the validMinutes list
    int index = validMinutes.indexOf(minutes);
    if (index == -1) {
        // If not exactly in the list (shouldn't happen after rounding), round it first
        roundMinutes(dateTimeEdit);
        return;
    }

    // Compute new index based on steps
    int newIndex = index + steps;
    if (newIndex < 0) {
        // Move to the previous hour if stepping down past 00
        dateTimeEdit->setTime(QTime(currentTime.hour() - 1, 45));
    } else if (newIndex >= validMinutes.size()) {
        // Move to the next hour if stepping up past 45
        dateTimeEdit->setTime(QTime(currentTime.hour() + 1, 0));
    } else {
        // Just set the new minute value
        dateTimeEdit->setTime(QTime(currentTime.hour(), validMinutes[newIndex]));
    }
}


