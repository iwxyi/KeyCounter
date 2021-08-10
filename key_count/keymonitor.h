#ifndef KEYCAPTURER_H
#define KEYCAPTURER_H

#include <QObject>
#include <QHash>

extern QHash<ulong, QString> KeyCodeNameMap;

class KeyMonitor : public QObject
{
    Q_OBJECT

public:
    virtual ~KeyMonitor();
    static KeyMonitor *&instance()
    {
        static KeyMonitor *s = nullptr;
        if (s == nullptr)
        {
            s = new KeyMonitor();
        }
        return s;
    }

    int startHook();

    bool stopHook();

public:
    void setkeyValue(ulong key);

protected:
    KeyMonitor();

signals:
    void getKey(int);
};

#endif // KEYCAPTURER_H
