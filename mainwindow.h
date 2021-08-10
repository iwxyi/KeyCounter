#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include "keycounter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initTray();

private slots:
    void trayAction(QSystemTrayIcon::ActivationReason reason);

    void on_actionFilePath_triggered();

    void on_actionExit_triggered();

    void on_actionGitHub_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    QSettings* settings;
    KeyCounter counter;
};
#endif // MAINWINDOW_H
