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

    //Window title
    setWindowTitle("CO2 Calculator");

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

    // setup necessary string list for the list widget
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

void MainWindow::paintItem(QListWidget* listWidget, QString itemValue, QColor paintColor){
    // paint Item in Color of graph line
        QList<QListWidgetItem*> items = listWidget->findItems(itemValue, Qt::MatchExactly);
        if (!items.isEmpty()) {
            QListWidgetItem* item = items.first();
            item->setBackground(paintColor);
        }
        else{
            qDebug() << "Item not found!";
            return;
        }

    return;
}


void MainWindow::on_applySettingsBtn_clicked()
{
    // Conversion values for frontend->backend translation
    std::unordered_map<QString, std::string> valConversion{
        {"Generated Power in Megawatt", "Generation_MW"},
        {"Emitted CO2 Emissions in tons", "CO2Emissions_gCO2eq"}
    };

    // String for Data Request
    QStringList selectedPsr = getCheckedItems(ui->listGenerationType);
    QStringList selectedVal = getCheckedItems((ui->listRequestValues));
    QString selectedDomain = ui->chooseCountryBox->currentText();
    QString selectedStart = ui->startTime->dateTime().toString("yyyyMMddHHmm");
    QString selectedEnd = ui->endTime->dateTime().toString("yyyyMMddHHmm");

    // Initiliase provider for access to data
    DataProvider DataProvider1;
    std::pair<std::vector<std::string>, std::vector<double>> requestData;

    if (!ui->plotWidget) {
        qDebug() << "Fehler: ui->plotWidget ist NULL!";
        return;
    }

    if (!ui->plotWidget->layout()) {
        QVBoxLayout *layout = new QVBoxLayout(ui->plotWidget);
        ui->plotWidget->setLayout(layout);
    }

    // delete existing plot
    QLayoutItem* item;
    while ((item = ui->plotWidget->layout()->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // reset color highlighting
    for (int i = 0; i < ui->listGenerationType->count(); ++i) {
        QListWidgetItem* item = ui->listGenerationType->item(i);
        item->setBackground(QBrush());
    }

    QCustomPlot *customPlot = new QCustomPlot(this);
    ui->plotWidget->layout()->addWidget(customPlot);

    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("dd.MM.yyyy HH:mm");
    dateTicker->setTickCount(5);

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
    } else {
        customPlot->xAxis->setTicker(dateTicker);
        customPlot->xAxis->setLabel("Date");
        if (!selectedVal.isEmpty())
            customPlot->yAxis->setLabel(selectedVal[0]);
    }

    //Requesting and plotting data
    for (int i = 0; i < selectedVal.size(); i++) {
        for (int j = 0; j < selectedPsr.size(); j++) {
            requestData = DataProvider1.get_data(
                selectedStart.toStdString(), selectedEnd.toStdString(),
                selectedPsr[j].toStdString(), valConversion[selectedVal[i]],
                selectedDomain.toStdString(), "Actual generation per type", "Realised");

            QVector<double> xTimeStamps = toUnixVector(requestData.first);
            QVector<double> yData(requestData.second.begin(), requestData.second.end());

            QCPGraph *graph = nullptr;
            QColor graphColor = getColor(j);

            if (selectedVal.size() == 2) {
                if (i == 0 && topPlot) {
                    graph = customPlot->addGraph(topPlot->axis(QCPAxis::atBottom), topPlot->axis(QCPAxis::atLeft));
                } else if (i == 1 && bottomPlot) {
                    graph = customPlot->addGraph(bottomPlot->axis(QCPAxis::atBottom), bottomPlot->axis(QCPAxis::atLeft));
                }
            } else {
                graph = customPlot->addGraph(customPlot->xAxis, customPlot->yAxis);
            }

            if (graph) {
                graph->setData(xTimeStamps, yData);
                graph->setLineStyle(QCPGraph::lsStepRight);
                graph->setPen(QPen(graphColor));
                graphList.append(graph);
                paintItem(ui->listGenerationType, selectedPsr[j], graphColor);
            }
        }
    }

    // Rescale properly and apply zero point for yAxis
    customPlot->rescaleAxes();

    if (selectedVal.size() == 2) {
        if (topPlot) {
            QCPRange range = topPlot->axis(QCPAxis::atLeft)->range();
            topPlot->axis(QCPAxis::atLeft)->setRange(qMin(0.0, range.lower), range.upper);
        }
        if (bottomPlot) {
            QCPRange range = bottomPlot->axis(QCPAxis::atLeft)->range();
            bottomPlot->axis(QCPAxis::atLeft)->setRange(qMin(0.0, range.lower), range.upper);
        }
    } else {
        QCPRange range = customPlot->yAxis->range();
        customPlot->yAxis->setRange(qMin(0.0, range.lower), range.upper);
    }

    customPlot->replot();
    customPlot->show();
}

void MainWindow::roundMinutes(QDateTimeEdit *dateTimeEdit) {
    // round entered time to 15-minute intervalls
    QTime currentTime = dateTimeEdit->time();
    int minutes = currentTime.minute();

    int roundedMinutes = (minutes + 7) / 15 * 15;
    if (roundedMinutes >= 60) {
        dateTimeEdit->setTime(QTime(currentTime.hour() + 1, 0));
    } else {
        dateTimeEdit->setTime(QTime(currentTime.hour(), roundedMinutes));
    }
}

void MainWindow::stepByMinutes(QDateTimeEdit *dateTimeEdit, int steps) {
    // creating custom stepBy function for dataTimeEdit Widget
    QTime currentTime = dateTimeEdit->time();
    int minutes = currentTime.minute();
    QList<int> validMinutes = {0, 15, 30, 45};

    // Find the current minute position in the validMinutes list
    int index = validMinutes.indexOf(minutes);
    if (index == -1) {
        // If not exactly in the list, round it first
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
    // Conversion to unix double time stamps for qcustomplots
    QVector<double> unixTimestamps;

    for (const std::string &timeString : timeStrings) {
        QDateTime dateTime = QDateTime::fromString(QString::fromStdString(timeString), "yyyyMMddHHmm");
        dateTime.setTimeZone(QTimeZone::UTC); // Falls UTC benötigt wird
        unixTimestamps.append(static_cast<double>(dateTime.toSecsSinceEpoch()));
    }

    return unixTimestamps;
}

QColor MainWindow::getColor(int pos) {
    // List of Colors for plotting
    QVector<QColor> colors = {
        QColor(255, 0, 0),      // Red
        QColor(0, 255, 0),      // Green
        QColor(0, 0, 255),      // Blue
        QColor(255, 255, 0),    // Yellow
        QColor(255, 165, 0),    // Orange
        QColor(128, 0, 128),    // Purple
        QColor(0, 255, 255),    // Cyan
        QColor(255, 192, 203),  // Pink
        QColor(165, 42, 42),    // Brown
        QColor(128, 128, 128),  // Gray
        QColor(0, 128, 0),      // DarkGreen
        QColor(0, 0, 128),      // Navy
        QColor(255, 105, 180),  // HotPink
        QColor(255, 215, 0),    // Gold
        QColor(0, 191, 255),    // DeepSkyBlue
        QColor(124, 252, 0),    // LawnGreen
        QColor(255, 140, 0),    // DarkOrange
        QColor(60, 179, 113),   // MediumSeaGreen
        QColor(138, 43, 226),   // BlueViolet
        QColor(255, 99, 71)     // Tomato
    };
    return colors[pos];
}

