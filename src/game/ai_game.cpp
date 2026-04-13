#include "ai_game.h"

AIGame::AIGame(QObject *parent) : Game(parent), humanPlayer(nullptr), aiPlayer(nullptr), aiPlayerIndex(1), humanPlayerIndex(0)
{
    // 人类玩家作为黑方（索引0），AI作为白方（索引1）
    humanPlayer = new GomokuPlayer(GomokuBoard::BLACK);
    aiPlayer = new AIPlayer(GomokuBoard::WHITE);
}

AIGame::~AIGame()
{
    delete humanPlayer;
    delete aiPlayer;
}

void AIGame::startNewGame()
{
    board.reset();
    currentPlayerIndex = 0; // 黑方先下
    gameStatus = GAME_IN_PROGRESS;
    winner = 0;
    moveCount = 0;
}

bool AIGame::makeMove(int x, int y)
{
    if (gameStatus != GAME_IN_PROGRESS) {
        return false;
    }

    // 只有人类玩家可以手动落子
    if (currentPlayerIndex == humanPlayerIndex) {
        int currentPlayerColor = humanPlayer->getColor();
        if (board.placeStone(x, y, currentPlayerColor)) {
            // 记录人类玩家的落子
            humanPlayer->addMove(x, y);
            
            moveCount++;
            if (board.checkWin(x, y, currentPlayerColor)) {
                gameStatus = GAME_FINISHED;
                winner = currentPlayerColor;
                emit gameOver(winner);
            } else if (board.isFull()) {
                gameStatus = GAME_FINISHED;
                winner = 0; // 平局
                emit gameOver(winner);
            } else {
                currentPlayerIndex = aiPlayerIndex; // 轮到AI
                emit moveMade(); // 发送信号通知UI更新
                
                // AI自动落子
                Move aiMove = aiPlayer->aiDecision(&board);
                makeMove(aiMove.x, aiMove.y);
            }
            return true;
        }
    } else if (currentPlayerIndex == aiPlayerIndex) {
        // AI落子
        int currentPlayerColor = aiPlayer->getColor();
        if (board.placeStone(x, y, currentPlayerColor)) {
            // AI不需要记录落子
            
            moveCount++;
            if (board.checkWin(x, y, currentPlayerColor)) {
                gameStatus = GAME_FINISHED;
                winner = currentPlayerColor;
                emit gameOver(winner);
            } else if (board.isFull()) {
                gameStatus = GAME_FINISHED;
                winner = 0; // 平局
                emit gameOver(winner);
            } else {
                currentPlayerIndex = humanPlayerIndex; // 轮到人类玩家
                emit moveMade(); // 发送信号通知UI更新
            }
            return true;
        }
    }
    return false;
}

bool AIGame::undoMove()
{
    if (gameStatus != GAME_IN_PROGRESS || moveCount == 0) {
        return false;
    }
    
    // 回退到上一个玩家
    int prevPlayerIndex = (currentPlayerIndex + 1) % 2;
    
    // 只有人类玩家可以悔棋
    if (prevPlayerIndex == humanPlayerIndex) {
        if (!humanPlayer->hasMoves()) {
            return false;
        }
        
        // 获取上一步落子
        Move move = humanPlayer->undoMove();
        if (move.x == -1 || move.y == -1) {
            return false;
        }
        
        // 从棋盘上移除棋子
        board.removeStone(move.x, move.y);
        
        // 更新游戏状态
        moveCount--;
        currentPlayerIndex = prevPlayerIndex;
        
        // 发送信号通知UI更新
        emit moveMade();
        
        return true;
    }
    return false;
}

int AIGame::getCurrentPlayer() const
{
    return currentPlayerIndex;
}

int AIGame::getGameStatus() const
{
    return gameStatus;
}

int AIGame::getWinner() const
{
    return winner;
}

GomokuBoard* AIGame::getBoard()
{
    return &board;
}

int AIGame::getMoveCount() const
{
    return moveCount;
}