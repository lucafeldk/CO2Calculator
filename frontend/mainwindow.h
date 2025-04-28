#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <unordered_map>
#include <QDateTimeEdit>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setupCheckBoxList(QListWidget* listWidget, QStringList items);

private slots:
    void on_applySettingsBtn_clicked();

    QStringList getCheckedItems(QListWidget* listWidget);

    void roundMinutes(QDateTimeEdit *dateTimeEdit);

    void stepByMinutes(QDateTimeEdit *dateTimeEdit, int steps);

    QVector<double> toUnixVector(std::vector<std::string> timeStrings);

    QColor getColor(int pos);

    void paintItem(QListWidget* listWidget, QString itemValue, QColor paintColor);

    void updateMaxTime();

    //void createBarPlot(std::unordered_map<)

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
