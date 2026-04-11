#include "gomoku_widget.h"
#include "gomoku_game.h"
#include "gomoku_board.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QVBoxLayout>
#include "gomoku_constants.h"
#include "board_widget.h"
using namespace GomokuConst;

GomokuWidget::GomokuWidget(QWidget *parent) : QWidget(parent)
{
    // 初始化游戏对象
    game = std::make_shared<GomokuGame>();

    // 连接信号与槽
    connect(game.get(), &GomokuGame::gameOver, this, &GomokuWidget::onGameOver);
    connect(game.get(), &GomokuGame::moveMade, this, &GomokuWidget::onMoveMade);

    // 棋盘宽度
    int boardWidth = MARGIN * 2 + CELL_SIZE * (BOARD_SIZE - 1);

    // 创建状态标签
    statusLabel = new QLabel("当前回合：黑方 (回合 0)", this);
    statusLabel->setFixedWidth(boardWidth);
    statusLabel->setAlignment(Qt::AlignCenter);

    // 创建棋盘小部件
    boardWidget = new BoardWidget(game, this);

    // 创建垂直布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(statusLabel);
    layout->addWidget(boardWidget);
    layout->setContentsMargins(0, 10, 0, 10);

    statusLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    
    boardWidget->setFixedSize(boardWidth, boardWidth);

    int labelHeight = statusLabel->sizeHint().height();
    int totalHeight = labelHeight + boardWidth + layout->spacing() + 20;

    setFixedSize(boardWidth, totalHeight);

    // 初始更新状态
    updateStatus();
}

GomokuWidget::~GomokuWidget(){}

void GomokuWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

}

void GomokuWidget::onMoveMade()
{
    // 落子成功后更新状态
    updateStatus();
}

void GomokuWidget::updateStatus()
{
    int currentPlayerIndex = game->getCurrentPlayer();
    int moveCount = game->getMoveCount();
    QString playerColor = currentPlayerIndex == 0 ? "黑方" : "白方";
    statusLabel->setText(QString("当前回合：%1 (回合 %2)").arg(playerColor).arg(moveCount));
}

void GomokuWidget::onGameOver(int winner)
{
    if (winner == 0) {
        QMessageBox::information(this, "游戏结束", "平局！");
    } else {
        QString winnerStr = winner == 1 ? "黑方" : "白方";
        QMessageBox::information(this, "游戏结束", winnerStr + "获胜！");
    }
}

void GomokuWidget::startNewGame()
{
    // 调用游戏对象的方法重置游戏
    game->startNewGame();

    // 更新状态并重绘棋盘
    updateStatus();
    update();
}