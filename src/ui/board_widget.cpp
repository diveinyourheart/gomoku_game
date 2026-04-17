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

// Gemini version
void BoardWidget::drawRealisticStone(QPainter& painter, int x, int y, bool isBlack) {
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 定义棋子半径
    int radius = CELL_SIZE / 2 - 2;

    // 设置径向渐变
    // 渐变中心稍微偏向左上方 (x - radius/3, y - radius/3) 以模拟光源
    // 焦点 (Focal Point) 设在同一位置可以增强高光聚集感
    QRadialGradient gradient(x - radius / 3.0, y - radius / 3.0, radius * 1.5);
    gradient.setFocalPoint(x - radius / 3.0, y - radius / 3.0);

    if (isBlack) {
        // 黑子渐变：从浅灰（高光）到深黑（暗面）
        gradient.setColorAt(0, QColor(80, 80, 80));   // 高光点
        gradient.setColorAt(0.3, QColor(20, 20, 20)); // 中间色
        gradient.setColorAt(1, QColor(0, 0, 0));      // 边缘暗部
    } else {
        // 白子渐变：从纯白（高光）到淡灰（阴影）
        gradient.setColorAt(0, QColor(255, 255, 255)); // 高光点
        gradient.setColorAt(0.6, QColor(230, 230, 230)); // 表面
        gradient.setColorAt(1, QColor(180, 180, 180));   // 边缘阴影
    }

    // 绘制阴影（可选，增加悬浮感）
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 60)); // 半透明深色
    painter.drawEllipse(x - radius + 2, y - radius + 2, radius * 2, radius * 2);

    // 绘制棋子主体
    painter.setBrush(gradient);
    painter.setPen(QPen(isBlack ? Qt::black : QColor(150, 150, 150), 1));
    painter.drawEllipse(x - radius, y - radius, radius * 2, radius * 2);
}

void BoardWidget::highlightStone(QPainter& painter, int x, int y) {
    // --- 方案 B: 柔和的外围光晕 (增强存在感) ---
    // 如果你坚持要红色，建议调低透明度并使用虚线，或者稍微扩大的圆环
    painter.setBrush(Qt::NoBrush);
    QPen highlightPen(QColor(255, 0, 0, 150), 2); // 150是透明度
    highlightPen.setStyle(Qt::DotLine); // 使用点状虚线更灵动
    painter.setPen(highlightPen);
    
    int ringSize = CELL_SIZE - 2; // 比棋子稍微大一圈
    painter.drawEllipse(x - ringSize / 2, y - ringSize / 2, ringSize, ringSize);
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

                    // painter.setPen(QPen(Qt::black, 1));
                    // if (stone == 1) {
                    //     painter.setBrush(Qt::black);
                    // } else {
                    //     painter.setBrush(Qt::white);
                    // }

                    drawRealisticStone(painter, x, y, stone == 1);

                    // 绘制棋子
                    painter.drawEllipse(x - CELL_SIZE / 2 + 2, y - CELL_SIZE / 2 + 2, CELL_SIZE - 4, CELL_SIZE - 4);

                    // 检查是否是最近一次落子，如果是则绘制高亮效果
                    if (i == turnFeature.move.x && j == turnFeature.move.y) {
                        // // 绘制高亮边框
                        // painter.setBrush(Qt::NoBrush);
                        // painter.setPen(QPen(Qt::red, 2));
                        // painter.drawEllipse(x - CELL_SIZE / 2, y - CELL_SIZE / 2, CELL_SIZE, CELL_SIZE);

                        highlightStone(painter, x, y);
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
