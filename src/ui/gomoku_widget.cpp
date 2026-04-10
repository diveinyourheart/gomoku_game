#include "gomoku_widget.h"
#include "gomoku_game.h"
#include "gomoku_board.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QVBoxLayout>

GomokuWidget::GomokuWidget(QWidget *parent) : QWidget(parent)
{
    // 初始化游戏对象
    game = new GomokuGame();

    // 连接信号与槽
    connect(game, &GomokuGame::gameOver, this, &GomokuWidget::onGameOver);
    connect(game, &GomokuGame::moveMade, this, &GomokuWidget::onMoveMade);

    // 创建状态标签
    statusLabel = new QLabel("当前回合：黑方 (回合 0)", this);
    statusLabel->setAlignment(Qt::AlignCenter);

    // 创建垂直布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(statusLabel);
    layout->setContentsMargins(0, 10, 0, 10);

    // 设置窗口大小
    int boardWidth = MARGIN * 2 + CELL_SIZE * (BOARD_SIZE - 1);
    int totalHeight = boardWidth + 40; // 额外空间用于状态标签
    setMinimumSize(boardWidth, totalHeight);
    setMaximumSize(boardWidth, totalHeight);

    // 初始更新状态
    updateStatus();
}

GomokuWidget::~GomokuWidget()
{
    delete game;
}

void GomokuWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    // 绘制棋盘（向下偏移 40 像素，为状态标签留出空间）
    painter.translate(0, 40);
    drawBoard(painter);
    drawStones(painter);
}

void GomokuWidget::mousePressEvent(QMouseEvent *event)
{
    // 计算点击位置对应的棋盘坐标
    int x = (event->x() - MARGIN + CELL_SIZE / 2) / CELL_SIZE;
    int y = (event->y() - MARGIN - 40 + CELL_SIZE / 2) / CELL_SIZE; // 调整 y 坐标，因为添加了状态标签

    // 检查坐标是否有效并落子
    if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
        if (game->makeMove(x, y)) {
            // 落子成功，重绘棋盘
            // 状态更新由 game 的 moveMade 信号触发
            update();
        }
    }
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

void GomokuWidget::drawBoard(QPainter &painter)
{
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::black, 1));

    // 绘制棋盘网格
    for (int i = 0; i < BOARD_SIZE; ++i) {
        painter.drawLine(MARGIN, MARGIN + i * CELL_SIZE, MARGIN + (BOARD_SIZE - 1) * CELL_SIZE, MARGIN + i * CELL_SIZE);
        painter.drawLine(MARGIN + i * CELL_SIZE, MARGIN, MARGIN + i * CELL_SIZE, MARGIN + (BOARD_SIZE - 1) * CELL_SIZE);
    }

    // 绘制棋盘中心的五个点
    int center = MARGIN + 7 * CELL_SIZE;
    int points[5][2] = {{3, 3}, {3, 11}, {7, 7}, {11, 3}, {11, 11}};
    for (int i = 0; i < 5; ++i) {
        int x = MARGIN + points[i][0] * CELL_SIZE;
        int y = MARGIN + points[i][1] * CELL_SIZE;
        painter.setBrush(Qt::black);
        painter.drawEllipse(x - 3, y - 3, 6, 6);
    }
}

void GomokuWidget::drawStones(QPainter &painter)
{
    painter.setRenderHint(QPainter::Antialiasing);

    // 从游戏对象获取棋盘
    GomokuBoard *board = game->getBoard();

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            int stone = board->getStone(i, j);
            if (stone != 0) {
                int x = MARGIN + i * CELL_SIZE;
                int y = MARGIN + j * CELL_SIZE;

                if (stone == 1) {
                    painter.setBrush(Qt::black);
                } else {
                    painter.setBrush(Qt::white);
                    painter.setPen(QPen(Qt::black, 1));
                }

                painter.drawEllipse(x - CELL_SIZE / 2 + 2, y - CELL_SIZE / 2 + 2, CELL_SIZE - 4, CELL_SIZE - 4);
            }
        }
    }
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