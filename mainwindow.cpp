#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      settings(new QSettings(QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat)),
      counter(settings, this)
{
    ui->setupUi(this);

    // 监听按键事件
    connect(KeyMonitor::instance(), &KeyMonitor::getKey, [=](ulong key) {
                counter.addKey(key);

                // 刷新界面
                if (this->hasFocus())
                {
                    update(); // 刷新计数
                }
            });
    KeyMonitor::instance()->startHook();

    initTray();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initTray()
{
    QSystemTrayIcon* tray = new QSystemTrayIcon(this);
    tray->setIcon(QIcon(":/icons/appicon"));
    tray->setToolTip("KeyCounter");
    tray->show();

    connect(tray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(trayAction(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::trayAction(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::Trigger:
        if (!this->isHidden())
        {
            this->hide();
        }
        else
        {
            this->showNormal();
            this->activateWindow();
        }
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    case QSystemTrayIcon::Context:
    {
        QMenu* menu = new QMenu(this);
        menu->addAction(ui->actionExit);
        menu->exec(QCursor::pos());
    }
        break;
    default:
        break;
    }
}


void MainWindow::on_actionFilePath_triggered()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(QApplication::applicationDirPath()));
}

void MainWindow::on_actionExit_triggered()
{
    qApp->exit();
}

void MainWindow::on_actionGitHub_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/iwxyi/KeyCounter"));
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "关于", "一个简单的按键统计");
}
