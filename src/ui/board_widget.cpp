#include "board_widget.h"
#include "game.h"
#include "gomoku_game.h"
#include "ai_game.h"
#include "gomoku_board.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include "gomoku_constants.h"
using namespace GomokuConst;

BoardWidget::BoardWidget(std::shared_ptr<Game> game, QWidget* parent)
    : QWidget(parent), game(game){
        if (game) {
            game->startNewGame();
            connect(game.get(), &Game::moveMade, this, &BoardWidget::onMoveMade);
        }
    }

void BoardWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    drawBoard(painter);
    drawStones(painter);
}

void BoardWidget::mousePressEvent(QMouseEvent* event)
{
    if (!game) return;
    
    if(game->canPlayerMove()){
        qDebug() << "clicked board" << event->x() << "," << event->y();
    
        int x = (event->x() - MARGIN + CELL_SIZE / 2) / CELL_SIZE;
        int y = (event->y() - MARGIN + CELL_SIZE / 2) / CELL_SIZE;
        qDebug() << "The coordinates of this move are:" << x << "," << y;

        if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
            if (game->makeMove(x, y)) {
                update();
            }
        }

        qDebug() << "The current game status is:" << game->getGameStatus();
        qDebug() << "The current move count is:" << game->getMoveCount();
        qDebug() << "The current player is:" << game->getCurrentPlayer();
        game->getBoard()->printBoard();
    }
}

void BoardWidget::drawBoard(QPainter& painter){
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
void BoardWidget::drawStones(QPainter& painter){
    painter.setRenderHint(QPainter::Antialiasing);

    // 从游戏对象获取棋盘
    if (game) {
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
}

void BoardWidget::onMoveMade(){
    update();
}

void BoardWidget::setGame(std::shared_ptr<Game> game){
    // 断开旧的信号槽连接
    if (this->game) {
        disconnect(this->game.get(), &Game::moveMade, this, &BoardWidget::onMoveMade);
    }
    
    // 更新游戏对象
    this->game = game;
    
    // 连接新的信号槽
    if (game) {
        connect(game.get(), &Game::moveMade, this, &BoardWidget::onMoveMade);
    }
    
    // 重绘棋盘
    update();
}
