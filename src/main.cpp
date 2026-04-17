#include <QApplication>
#include "mainwindow.h"
#include "gomoku_qdebug_log.h"
#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char *argv[])
{
    #ifdef _WIN32
    // 设置控制台输出为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
    #endif
    Logger::instance().addSink(std::make_shared<QtSink>());
    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(":/icons/app_icon.svg"));

    MainWindow w;
    w.show();
    return a.exec();
}