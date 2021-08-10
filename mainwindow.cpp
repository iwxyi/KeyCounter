#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      settings(new QSettings(QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat)),
      counter(settings, this)
{
    ui->setupUi(this);

    // 连接事件
    connect(KeyMonitor::instance(), &KeyMonitor::getKey, [=](ulong key) {
                counter.addKey(key);

                // 刷新界面
            });
    KeyMonitor::instance()->startHook();
}

MainWindow::~MainWindow()
{
    delete ui;
}

