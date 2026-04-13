#ifdef _WIN32
#include <windows.h>
#endif
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    #ifdef _WIN32
    // 设置控制台输出为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
    #endif
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}