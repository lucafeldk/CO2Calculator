#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QCheckBox>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QWidget>
#include "DataProvider.h"
#include <vector>
#include <QVector>
#include "qcustomplot.h"
#include "unordered_map"


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
        "Biomass", "Fossil Brown coal", "Fossil Coal-derived gas",
        "Fossil Gas", "Fossil Hard coal", "Fossil Oil", "Fossil Peat",
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
        "Germany",
        "France"
    };

    // setup the checkbox widgets
    setupCheckBoxList(ui->listGenerationType,generationList);
    setupCheckBoxList(ui->listRequestValues,requestValues);

    // setup the time widgets
    QDateTime minDateTime(QDate(2015, 1, 1), QTime(0, 0));
    QDateTime defaultStartTime(QDate(2020,1,1), QTime(0,0));
    ui->startTime->setMinimumDateTime(minDateTime);
    ui->startTime->setDateTime(defaultStartTime);
    ui->endTime->setDateTime(defaultStartTime.addDays(1));

    // setup the selecting country widget
    ui ->chooseCountryBox->clear();
    for (const QString &itemText : inDomain){
        ui ->chooseCountryBox->addItem(itemText);
    }
    ui->chooseCountryBox->setCurrentText("Germany"); // set default value
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

    std::unordered_map<QString, std::string> valConversion{
        {"Generated Power in Megawatt", "Generation_MW"},
        {"Emitted CO2 Emissions in tons", "CO2Emissions_gCO2eq"}
    };

    // read in the parameters for the request
    QStringList selectedPsr = getCheckedItems(ui->listGenerationType);
    QStringList selectedVal = getCheckedItems((ui->listRequestValues));
    QString selectedDomain = ui->chooseCountryBox->currentText();
    QString selectedStart = ui->startTime->dateTime().toString("yyyyMMddHHmm");
    QString selectedEnd = ui->endTime->dateTime().toString("yyyyMMddHHmm");


    if (!ui->plotWidget) {
        qDebug() << "Fehler: ui->plotWidget ist NULL!";
        return;
    }

    if (!ui->plotWidget->layout()) {
        QVBoxLayout *layout = new QVBoxLayout(ui->plotWidget);
        ui->plotWidget->setLayout(layout);
    }

    // delete the current data of the plot
    QLayoutItem* item;
    while ((item = ui->plotWidget->layout()->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Create Plot and add to widget
    QCustomPlot *customPlot = new QCustomPlot(this);
    ui->plotWidget->layout()->addWidget(customPlot);

    //Data Provider for requesting the Data
    DataProvider DataProvider1;
    std::pair<std::vector<std::string>, std::vector<double>> requestData;

    // **X-Achse als Datumsformat setzen**
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("dd.MM.yyyy HH:mm");
    dateTicker->setTickCount(5);
    customPlot->xAxis->setTicker(dateTicker);

    // Plot components
    QCPAxisRect *topPlot = nullptr;
    QCPAxisRect *bottomPlot = nullptr;
    QVector<QCPGraph*> graphList;

    if (selectedVal.size() == 2) {
        customPlot->plotLayout()->clear();
        topPlot = new QCPAxisRect(customPlot);
        bottomPlot = new QCPAxisRect(customPlot);
        topPlot->axis(QCPAxis::atBottom)->setTicker(dateTicker);
        topPlot->axis(QCPAxis::atBottom)->setLabel("Date");
        topPlot->axis(QCPAxis::atLeft)->setLabel("Generated Power in Megawatt");
        bottomPlot->axis(QCPAxis::atBottom)->setTicker(dateTicker);
        bottomPlot->axis(QCPAxis::atBottom)->setLabel("Date");
        bottomPlot->axis(QCPAxis::atLeft)->setLabel("Emitted CO2 Emissions in tons");

        customPlot->plotLayout()->addElement(0, 0, topPlot);
        customPlot->plotLayout()->addElement(1, 0, bottomPlot);
    }

    // Plotting the Data
    for (int i = 0; i < selectedVal.size(); i++) {
        for (QString &psrType : selectedPsr) {
            requestData = DataProvider1.get_data(selectedStart.toStdString(), selectedEnd.toStdString(),
                                                 psrType.toStdString(), valConversion[selectedVal[i]],
                                                 selectedDomain.toStdString(), "Actual generation per type", "Realised");

            QVector<double> xTimeStamps = toUnixVector(requestData.first);
            QVector<double> yData(requestData.second.begin(), requestData.second.end());

            QCPGraph *graph = new QCPGraph(customPlot->xAxis, customPlot->yAxis);
            if (selectedVal.size()==2 && i == 0){ // topPlot
                graph->setKeyAxis(topPlot->axis(QCPAxis::atBottom));
                graph->setValueAxis(topPlot->axis(QCPAxis::atLeft));
                graph->setData(xTimeStamps, yData);
                graph->setLineStyle(QCPGraph::lsStepRight);
                graphList.append(graph);
            }
            else if (selectedVal.size()==2 && i==1){ //bottom Plot
                graph->setKeyAxis(bottomPlot->axis(QCPAxis::atBottom));
                graph->setValueAxis(bottomPlot->axis(QCPAxis::atLeft));
                graph->setData(xTimeStamps, yData);
                graph->setLineStyle(QCPGraph::lsStepRight);
                graphList.append(graph);
            }
            else{
                graph->setData(xTimeStamps, yData);
                graph->setLineStyle(QCPGraph::lsStepRight);
                customPlot->yAxis->setLabel(selectedVal[i]);
                customPlot->xAxis->setLabel("Date");
            }
        }
    }
    customPlot->rescaleAxes();
    customPlot->replot();
    customPlot->show();
}


void MainWindow::on_listGenerationType_itemClicked(QListWidgetItem *item)
{

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

QVector<double> MainWindow::toUnixVector(std::vector<std::string> timeStrings) {
    QVector<double> unixTimestamps;  // Ändere den Rückgabewert zu QVector<double>

    for (const std::string &timeString : timeStrings) {
        QDateTime dateTime = QDateTime::fromString(QString::fromStdString(timeString), "yyyyMMddHHmm");
        dateTime.setTimeZone(QTimeZone::UTC); // Falls UTC benötigt wird
        unixTimestamps.append(static_cast<double>(dateTime.toSecsSinceEpoch())); // Umwandlung zu double
    }

    return unixTimestamps;
}


