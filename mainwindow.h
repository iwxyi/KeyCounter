#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QStandardItemModel>
#include "keycounter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    void initView();
    void initTray();
    void initTable();
    void hideKeypad(bool hide);

private slots:
    void trayAction(QSystemTrayIcon::ActivationReason reason);

    void on_actionFilePath_triggered();

    void on_actionExit_triggered();

    void on_actionGitHub_triggered();

    void on_actionAbout_triggered();

    void on_actionStartup_triggered();

    void on_actionKeypad_triggered();

    void on_actionRestartMonitor_triggered();

protected:
    void showEvent(QShowEvent* e) override;
    void closeEvent(QCloseEvent*e) override;

private:
    Ui::MainWindow *ui;
    QSettings* settings = nullptr;
    KeyCounter* counter = nullptr;
    QStandardItemModel* countModel = nullptr;
    QMap<ulong, int> tableRowMapping;

    const int Col_Key_Code = -1;
    const int Col_Key_Name = 0;
    const int Col_Key_Count = 1;
};
#endif // MAINWINDOW_H
