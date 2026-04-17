#include "gomoku_game.h"

GomokuGame::GomokuGame(QObject *parent)
    : Game(parent)
{
    players[0] = new GomokuPlayer(GomokuBoard::BLACK);
    players[1] = new GomokuPlayer(GomokuBoard::WHITE);
}

GomokuGame::~GomokuGame()
{
    delete players[0];
    delete players[1];
}

void GomokuGame::startNewGame()
{
    board.reset();
    currentPlayerIndex = 0;
    gameStatus = GAME_IN_PROGRESS;
    winner = 0;
    moveCount = 0;
    hasUndone = false; // 重置悔棋标志
    emit undoButtonStateChanged(false); // 游戏刚开始，禁用悔棋按钮
}

int GomokuGame::getMoveCount() const
{
    return moveCount;
}

bool GomokuGame::makeMove(int x, int y)
{
    if (gameStatus != GAME_IN_PROGRESS) {
        return false;
    }

    // 如果已经悔棋，重置悔棋标志
    if (hasUndone) {
        hasUndone = false;
        emit undoButtonStateChanged(true); // 启用悔棋按钮
    }

    int currentPlayerColor = players[currentPlayerIndex]->getColor();
    if (board.placeStone(x, y, currentPlayerColor)) {
        // 记录落子（仅对人类玩家）
        GomokuPlayer* humanPlayer = dynamic_cast<GomokuPlayer*>(players[currentPlayerIndex]);
        if (humanPlayer) {
            humanPlayer->addMove(x, y);
        }
        
        moveCount++;
        // 第一次落子后启用悔棋按钮
        if (moveCount == 1) {
            emit undoButtonStateChanged(true); // 启用悔棋按钮
        }
        if (board.checkWin(x, y, currentPlayerColor)) {
            gameStatus = GAME_FINISHED;
            winner = currentPlayerColor;
            emit gameOver(winner);
            emit undoButtonStateChanged(false); // 游戏结束，禁用悔棋按钮
        } else if (board.isFull()) {
            gameStatus = GAME_FINISHED;
            winner = 0; // 平局
            emit gameOver(winner);
            emit undoButtonStateChanged(false); // 游戏结束，禁用悔棋按钮
        } else {
            currentPlayerIndex = (currentPlayerIndex + 1) % 2;
            emit moveMade(); // 落子成功且游戏未结束，发送信号
        }
        return true;
    }
    return false;
}

bool GomokuGame::undoMove()
{
    if (gameStatus != GAME_IN_PROGRESS || moveCount == 0 || hasUndone) {
        return false;
    }
    
    // 回退到上一个玩家
    int prevPlayerIndex = (currentPlayerIndex + 1) % 2;
    
    // 检查是否为GomokuPlayer类型
    GomokuPlayer* humanPlayer = dynamic_cast<GomokuPlayer*>(players[prevPlayerIndex]);
    if (!humanPlayer || !humanPlayer->hasMoves()) {
        return false;
    }
    
    // 获取上一步落子
    PlayerMove move = humanPlayer->undoMove();
    if (move.x == -1 || move.y == -1) {
        return false;
    }
    
    // 从棋盘上移除棋子
    board.removeStone(move.x, move.y);
    
    // 更新游戏状态
    moveCount--;
    currentPlayerIndex = prevPlayerIndex;
    hasUndone = true; // 设置悔棋标志
    
    // 发送信号通知UI更新
    emit moveMade();
    emit undoButtonStateChanged(false); // 禁用悔棋按钮
    
    return true;
}

int GomokuGame::getCurrentPlayer() const
{
    return currentPlayerIndex;
}

int GomokuGame::getGameStatus() const
{
    return gameStatus;
}

int GomokuGame::getWinner() const
{
    return winner;
}

GomokuBoard* GomokuGame::getBoard()
{
    return &board;
}

bool GomokuGame::canPlayerMove() const
{
    return gameStatus == GAME_IN_PROGRESS;
}

Game::TurnFeature GomokuGame::getCurrentTurnFeature() const
{
    // 获取最后一步落子坐标
    PlayerMove lastMove(-1, -1);
    
    // 获取上一个玩家的索引
    int previousPlayerIndex = (currentPlayerIndex + 1) % 2;
    
    // 根据上一个玩家获取最后一步落子
    if (players[previousPlayerIndex] && players[previousPlayerIndex]->hasMoves()) {
        lastMove = players[previousPlayerIndex]->getLastMove();
    }
    
    return TurnFeature(moveCount, currentPlayerIndex, lastMove);
}