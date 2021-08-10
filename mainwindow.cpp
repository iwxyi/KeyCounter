#include <QDebug>
#include <QFile>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "keymonitor.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置要监控的键码
    QFile keyFile(":/documents/key_code.txt");
    keyFile.open(QIODevice::ReadOnly);
    QTextStream keyCodeIn(&keyFile);
    keyCodeIn.setCodec("UTF-8");
    QString line = keyCodeIn.readLine();
    while (!line.isNull())
    {
        QStringList sl = line.split("\t", QString::SkipEmptyParts);
        if (sl.size() >= 2)
        {
            KeyCodeMap.insert(static_cast<ulong>(sl.at(0).toInt()), sl.at(1));
        }
        line = keyCodeIn.readLine();
    }

    // 连接事件
    connect(KeyMonitor::instance(), &KeyMonitor::getKey, [=](ulong key) {
                qDebug() << "Hook_Key: " << key << KeyCodeMap.value(key);
            });
    KeyMonitor::instance()->startHook();
}

MainWindow::~MainWindow()
{
    KeyMonitor::instance()->stopHook();
    delete ui;
}

