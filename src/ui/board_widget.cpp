#include "board_widget.h"
#include "game.h"
#include "gomoku_game.h"
#include "ai_game.h"
#include "gomoku_board.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include "gomoku_constants.h"
#include "gomoku_qdebug_log.h"
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
        LOG_INFO("clicked board: ",event->x(),",",event->y());

    
        int x = (event->x() - MARGIN + CELL_SIZE / 2) / CELL_SIZE;
        int y = (event->y() - MARGIN + CELL_SIZE / 2) / CELL_SIZE;
        LOG_INFO("The coordinates of this move are: ",x,",",y);

        if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
            bool makeMoveSuccess = game->makeMove(x, y);
            if(!makeMoveSuccess){
                LOG_INFO("Invalid move");
            }
        }

        LOG_INFO("The current game status is: ",game->getGameStatus());
        LOG_INFO("The current move count is: ",game->getMoveCount());
        LOG_INFO("The current player is: ",game->getCurrentPlayer());
        LOG_INFO("The current board is: ",game->getBoard()->boardToString());
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
        // 获取当前回合特征，用于标识最近一次落子
        Game::TurnFeature turnFeature = game->getCurrentTurnFeature();
        LOG_INFO("The current turn feature is: ",turnFeature);

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

                    // 绘制棋子
                    painter.drawEllipse(x - CELL_SIZE / 2 + 2, y - CELL_SIZE / 2 + 2, CELL_SIZE - 4, CELL_SIZE - 4);

                    // 检查是否是最近一次落子，如果是则绘制高亮效果
                    if (i == turnFeature.move.x && j == turnFeature.move.y) {
                        // 绘制高亮边框
                        painter.setBrush(Qt::NoBrush);
                        painter.setPen(QPen(Qt::red, 2));
                        painter.drawEllipse(x - CELL_SIZE / 2, y - CELL_SIZE / 2, CELL_SIZE, CELL_SIZE);
                    }
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
