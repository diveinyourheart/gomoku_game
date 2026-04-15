#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "font_manager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentGameMode(GomokuConst::GameMode::None) // 初始化默认游戏模式为None
{
    ui->setupUi(this);

    // 设置字体
    ui->label_title->setFont(FontManager::getFont(FontManager::Title));
    ui->btn_normalMode->setFont(FontManager::getFont(FontManager::Button));
    ui->btn_aiMode->setFont(FontManager::getFont(FontManager::Button));
    ui->btn_Undo->setFont(FontManager::getFont(FontManager::Button));
    ui->menu_Game->setFont(FontManager::getFont(FontManager::Body));
    ui->menu_Help->setFont(FontManager::getFont(FontManager::Body));

    // 手动连接信号与槽
    connect(ui->action_New_Game, &QAction::triggered, this, &MainWindow::onNewGame);
    connect(ui->action_Exit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->action_About, &QAction::triggered, this, &MainWindow::onAbout);
    connect(ui->btn_normalMode, &QPushButton::clicked, this, &MainWindow::onBtnNormalModeClicked);
    connect(ui->btn_aiMode, &QPushButton::clicked, this, &MainWindow::onBtnAiModeClicked);
    connect(ui->action_Back_to_Menu, &QAction::triggered, this, &MainWindow::onActionBackToMenuTriggered);
    connect(ui->btn_Undo, &QPushButton::clicked, ui->gomokuWidget, &GomokuWidget::onUndoButtonClicked);
    connect(ui->gomokuWidget, &GomokuWidget::undoButtonStateChanged, this, &MainWindow::onUndoButtonStateChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onNewGame()
{
    // 根据当前游戏模式开始新游戏
    ui->gomokuWidget->startNewGame(currentGameMode);
}

void MainWindow::onAbout()
{
    QMessageBox::information(this, "关于", "五子棋游戏 v1.0\n\n使用 C++ 和 Qt 开发");
}

void MainWindow::onBtnNormalModeClicked()
{
    // 更新当前游戏模式
    currentGameMode = GomokuConst::GameMode::Normal;
    // 切换到游戏页面
    ui->stackedWidget->setCurrentIndex(1);
    // 开始新游戏
    ui->gomokuWidget->startNewGame(currentGameMode);
}

void MainWindow::onBtnAiModeClicked()
{
    // 更新当前游戏模式
    currentGameMode = GomokuConst::GameMode::AI;
    // 切换到游戏页面
    ui->stackedWidget->setCurrentIndex(1);
    // 开始新游戏
    ui->gomokuWidget->startNewGame(currentGameMode);
}

void MainWindow::onActionBackToMenuTriggered()
{
    // 切换回模式选择页面
    ui->stackedWidget->setCurrentIndex(0);
    // 重置游戏模式为None
    currentGameMode = GomokuConst::GameMode::None;
}

void MainWindow::onUndoButtonStateChanged(bool enabled)
{
    // 更新悔棋按钮的状态
    ui->btn_Undo->setEnabled(enabled);
}