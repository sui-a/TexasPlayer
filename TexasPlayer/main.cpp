#define _CRT_SECURE_NO_WARNINGS
#include "TexasPlayer.h"
#include <QtWidgets/QApplication>
#include <windows.h>
#include "signal.h"
#include "MusicSQL.h"

int main(int argc, char *argv[])
{
    if (!signal::setupSignalHandlers())
    {
		return 0; // 已有实例在运行，退出当前实例
    }

    //打开控制台
    if (AttachConsole(ATTACH_PARENT_PROCESS) == 0)
    {
        // 如果失败，则为自己分配一个新的控制台窗口
        AllocConsole();
    }
    //重定向标准输入输出流到控制台
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    freopen("CONIN$", "r", stdin);


    QApplication app(argc, argv);
    TexasPlayer window;
    window.show();
    return app.exec();
}
