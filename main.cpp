#include <QApplication>
#include "mainwindow.h"
// #include "qtsingleapplication/qtsingleapplication.h"

int main(int argc, char *argv[])
{
    QApplication::setDesktopSettingsAware(true); // 据说是避免不同分辨率导致显示的字体大小不一致
#if (QT_VERSION > QT_VERSION_CHECK(5, 6, 0))
    // 设置高分屏属性以便支持 2K4K 等高分辨率，尤其是手机 app。
    // 必须写在 main 函数的 QApplication a (argc, argv); 的前面
    // 设置后，读取到的窗口会随着显示器倍数而缩小
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("KeyCounter");
    QCoreApplication::setApplicationVersion("v0.1");

    // 我只想实现一个高效简约的按键统计，不想要消耗太多资源
    // 如果加上单例运行，则需要模块添加 Network
    /* const QString APP_ID("KeyCounter");// 单例ID
    QtSingleApplication a(APP_ID, argc, argv);
    if (a.isRunning())
        return !a.sendMessage("KCKCKC"); */

    a.setQuitOnLastWindowClosed(false); // 关闭最后一个窗口的时候程序不退出（菜单也算窗口）

    QFont font(a.font());
    font.setFamily("微软雅黑");
    font.setPointSize(qMax(10, font.pointSize()));
    a.setFont(font);

    MainWindow w;
    w.show();

    /* QObject::connect(&a, &QtSingleApplication::messageReceived, &w, [&](const QString&) {
        w.show(); // 单例自带的 setActivationWindow() 无法打开hide的窗口，得手动show一下才行
        w.activateWindow();
    }); */

    return a.exec();
}
