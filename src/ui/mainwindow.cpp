#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 手动连接信号与槽
    connect(ui->action_New_Game, &QAction::triggered, this, &MainWindow::onNewGame);
    connect(ui->action_Exit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->action_About, &QAction::triggered, this, &MainWindow::onAbout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onNewGame()
{
    ui->gomokuWidget->startNewGame();
}

void MainWindow::onAbout()
{
    QMessageBox::information(this, "关于", "五子棋游戏 v1.0\n\n使用 C++ 和 Qt 开发");
}