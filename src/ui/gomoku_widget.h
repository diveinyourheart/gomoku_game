#ifndef GOMOKU_WIDGET_H
#define GOMOKU_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <memory>
class Game;
class BoardWidget;
#include "gomoku_constants.h"
using namespace GomokuConst;

class GomokuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GomokuWidget(QWidget *parent = nullptr);
    ~GomokuWidget();

protected:
    void paintEvent(QPaintEvent *event) override;
    // void mousePressEvent(QMouseEvent *event) override;

private:

    std::shared_ptr<Game> game = nullptr; // 游戏逻辑对象
    QLabel *statusLabel; // 显示当前状态的标签
    BoardWidget *boardWidget; // 棋盘小部件

    void updateStatus(); // 更新状态显示

private slots:
    void onGameOver(int winner); // 处理游戏结束信号
    void onMoveMade(); // 处理落子成功信号
    void onUndoButtonStateChanged(bool enabled); // 处理悔棋按钮状态变化信号

public slots:
    void startNewGame(GomokuConst::GameMode mode = GomokuConst::GameMode::Normal);
    void onUndoButtonClicked();

 signals:
    void undoButtonStateChanged(bool enabled); // 转发悔棋按钮状态变化信号
};

#endif // GOMOKU_WIDGET_H