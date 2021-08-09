#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "keymonitor.h"
#include "keycapturer.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(KeyCapturer::instance(), &KeyCapturer::getKey, [=](int key) {
                qDebug() << "Hook_Key" << QString::number(key);
            });
    startHook();
}

MainWindow::~MainWindow()
{
    delete ui;
}

