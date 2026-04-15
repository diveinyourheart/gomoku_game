#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gomoku_constants.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNewGame();
    void onAbout();
    void onBtnNormalModeClicked();
    void onBtnAiModeClicked();
    void onActionBackToMenuTriggered();
    void onUndoButtonStateChanged(bool enabled);

private:
    Ui::MainWindow *ui;
    GomokuConst::GameMode currentGameMode; // 当前游戏模式
};

#endif // MAINWINDOW_H