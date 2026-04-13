#include <QApplication>
#include "mainwindow.h"
#include "env_util.h"

const std::string API_KEY = []() {
    EnvUtil::getInstance().loadEnvFile("../../.env");
    return EnvUtil::getInstance().getEnv("DEEPSEEK_API_KEY", "");
}();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}