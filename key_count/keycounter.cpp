#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "keycounter.h"

KeyCounter::KeyCounter(QSettings *settings, QObject *parent)
    : QObject(parent),
      settings(settings)
{
    // 设置启用的数组

    // 设置要监控的键码
    QFile keyFile(":/documents/key_code.txt");
    keyFile.open(QIODevice::ReadOnly);
    QTextStream keyCodeIn(&keyFile);
    keyCodeIn.setCodec("UTF-8");
    QString line;
    QString label;
    while (!(line = keyCodeIn.readLine().trimmed()).isNull())
    {
        if (line.startsWith("#"))
            continue;
        if (line.endsWith(":"))
        {
            label = line.left(line.length() - 1).trimmed();
            continue;
        }
        QStringList sl = line.split("\t", QString::SkipEmptyParts);
        if (sl.size() >= 2)
        {
            KeyCodeNameMap.insert(static_cast<ulong>(sl.at(0).toInt()), sl.at(1));
        }
    }

    // 恢复之前的键值
    for (auto key: KeyCodeNameMap.keys())
    {
        keyCountMap.insert(key, settings->value("count/" + QString::number(key), 0).toInt());
    }
}

KeyCounter::~KeyCounter()
{
    KeyMonitor::instance()->stopHook();
}

void KeyCounter::addKey(ulong keyCode)
{
    keyCountMap[keyCode]++;
    settings->setValue("count/" + QString::number(keyCode), keyCountMap.value(keyCode));
    // qDebug() << "key: " << keyCode << KeyCodeNameMap.value(keyCode) << keyCountMap[keyCode];
}

int KeyCounter::getKeyCount(ulong keyCode) const
{
    return keyCountMap.value(keyCode);
}
