#ifndef KEYCOUNTER_H
#define KEYCOUNTER_H

#include <QMap>
#include <QSettings>
#include "keymonitor.h"

class KeyCounter : QObject
{
public:
    KeyCounter(QSettings* settings, QObject* parent = nullptr);
    ~KeyCounter();

    void addKey(ulong keyCode);

    int getKey(ulong keyCode) const;

private:
    QSettings* settings;
    QMap<ulong, int> keyCountMap;
};

#endif // KEYCOUNTER_H
