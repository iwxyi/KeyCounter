#include "keymonitor.h"
#include <QtDebug>
#include "keycapturer.h"

LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam);

HMODULE WINAPI ModuleFromAddress(PVOID pv);
static HHOOK hHook;

LRESULT KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    KBDLLHOOKSTRUCT *Key_Info = (KBDLLHOOKSTRUCT *)(lParam);
    if (HC_ACTION == nCode)
    {
        if (WM_KEYDOWN == wParam || WM_SYSKEYDOWN == wParam) //如果按键为按下状态
        {
            if (Key_Info->vkCode <= 107 && Key_Info->vkCode >= 65)
            {
                qDebug() << Key_Info->vkCode;
                if (KeyCapturer::instance())
                {
                    KeyCapturer::instance()->setkeyValue(int(Key_Info->vkCode));
                }
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

int startHook()
{
    hHook = SetWindowsHookExW(WH_KEYBOARD_LL, KeyboardHookProc, ModuleFromAddress(PVOID(KeyboardHookProc)), 0);
    int error = int(GetLastError());
    return error;
}

bool stopHook()
{
    return UnhookWindowsHookEx(hHook);
}
