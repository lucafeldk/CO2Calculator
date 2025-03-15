#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <unordered_map>

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

    void on_listGenerationType_itemClicked(QListWidgetItem *item);

    void on_listRequestValues_itemClicked(QListWidgetItem *item);

    void on_fontComboBox_currentTextChanged(const QString &arg1);

    void on_startTime_dateTimeChanged(const QDateTime &dateTime);

    void on_endTime_dateTimeChanged(const QDateTime &dateTime);

    void on_screenshotBtn_clicked();

    void on_downloadButton_clicked();

    QStringList getCheckedItems(QListWidget* listWidget);

    //void createBarPlot(std::unordered_map<)

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
