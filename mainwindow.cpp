#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QDir>
#include <QTimer>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      settings(new QSettings(QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat)),
      counter(new KeyCounter(settings, this))
{
    ui->setupUi(this);

    // 监听按键事件
    connect(KeyMonitor::instance(), &KeyMonitor::getKey, [=](ulong key) {
                counter->addKey(key);
                countModel->setData(countModel->index(tableRowMapping[key], Col_Key_Count), QString::number(counter->getKeyCount(key)));

                // 刷新界面
                if (!this->isHidden())
                {
                    ui->countTable->update();
                }
            });

    // 初始化
    initView();
    initTray();
    initTable();

    if (!settings->value("mainwindow/hide", false).toBool())
        this->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initView()
{
    // 设置菜单
    ui->actionStartup->setChecked(settings->value("runtime/reboot", false).toBool());
    ui->actionKeypad->setChecked(settings->value("counter/keypad", true).toBool());
}

void MainWindow::initTray()
{
    QSystemTrayIcon* tray = new QSystemTrayIcon(this);
    tray->setIcon(QIcon(":/icons/appicon"));
    tray->setToolTip("KeyCounter");
    tray->show();

    connect(tray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(trayAction(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::initTable()
{
    if (countModel)
        countModel->deleteLater();
    tableRowMapping.clear();

    // 创建Model
    countModel = new QStandardItemModel(this);
    countModel->setColumnCount(2);
    countModel->setHorizontalHeaderLabels({"键名", "次数"});
    countModel->setRowCount(KeyCodeNameMap.count());

    bool keypad = ui->actionKeypad->isChecked();

    // 设置表格值
    auto keys = KeyCodeNameMap.keys();
    std::sort(keys.begin(), keys.end());
    int row = 0;
    for (ulong key: keys)
    {
        countModel->setItem(row, Col_Key_Name, new QStandardItem(KeyCodeNameMap.value(key)));
        countModel->setItem(row, Col_Key_Count, new QStandardItem(QString::number(counter->getKeyCount(key))));
        tableRowMapping[key] = row++;
    }
    ui->countTable->setModel(countModel);

    // 判断小键盘
    if (!keypad)
    {
        hideKeypad(true);
    }
}

void MainWindow::hideKeypad(bool hide)
{
    for (ulong key = 96; key <= 111; key++)
    {
        if (tableRowMapping.contains(key))
        {
            if (hide)
                ui->countTable->hideRow(tableRowMapping.value(key));
            else
                ui->countTable->showRow(tableRowMapping.value(key));
        }
    }
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

void MainWindow::showEvent(QShowEvent *e)
{
    restoreGeometry(settings->value("mainwindow/geometry").toByteArray());
    settings->setValue("mainwindow/hide", false);
    return QMainWindow::showEvent(e);
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    settings->setValue("mainwindow/geometry", saveGeometry());
    QTimer::singleShot(5000, [=]{
        settings->setValue("mainwindow/hide", true);
        settings->sync();
    });
    return QMainWindow::closeEvent(e);
}

void MainWindow::on_actionStartup_triggered()
{
    QString appName = QApplication::applicationName();
    QString appPath = QDir::toNativeSeparators(QApplication::applicationFilePath());
    QSettings *reg=new QSettings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    QString val = reg->value(appName).toString();// 如果此键不存在，则返回的是空字符串
    bool reboot = !settings->value("runtime/reboot", false).toBool();
    if (reboot)
        reg->setValue(appName, appPath);
    else
        reg->remove(appName);
    settings->setValue("runtime/reboot", reboot);
    ui->actionStartup->setChecked(reboot);
    qInfo() << "设置自启：" << reboot;
    reg->deleteLater();
}

void MainWindow::on_actionKeypad_triggered()
{
    bool keypad = ui->actionKeypad->isChecked();
    settings->setValue("counter/keypad", keypad);
    hideKeypad(!keypad);
    ui->countTable->update();
}
