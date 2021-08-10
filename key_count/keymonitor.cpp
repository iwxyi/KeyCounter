#include <windows.h>
#include <dbghelp.h>
#include <QDebug>
#include "keymonitor.h"

LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam);

HMODULE WINAPI ModuleFromAddress(PVOID pv);

static HHOOK hHook;

QHash<ulong, QString> KeyCodeNameMap;

LRESULT KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    KBDLLHOOKSTRUCT *Key_Info = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);
    if (HC_ACTION == nCode)
    {
        if (WM_KEYDOWN == wParam || WM_SYSKEYDOWN == wParam) //如果按键为按下状态
        {
            DWORD code = Key_Info->vkCode;
            // qDebug() << "key: " << code;
            if (KeyCodeNameMap.contains(code))
            {
                KeyMonitor::instance()->setkeyValue(code);
            }
        }
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

HMODULE ModuleFromAddress(PVOID pv)
{
    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQuery(pv, &mbi, sizeof(mbi)) != 0)
    {
        return HMODULE(mbi.AllocationBase);
    }
    else
    {
        return nullptr;
    }
}

// =================================================

KeyMonitor::KeyMonitor()
{
}

KeyMonitor::~KeyMonitor()
{
}

int KeyMonitor::startHook()
{
    hHook = SetWindowsHookExW(WH_KEYBOARD_LL, KeyboardHookProc, ModuleFromAddress(PVOID(KeyboardHookProc)), 0);
    int error = int(GetLastError());
    return error;
}

bool KeyMonitor::stopHook()
{
    return UnhookWindowsHookEx(hHook);
}

void KeyMonitor::setkeyValue(ulong key)
{
    emit getKey(key);
}
