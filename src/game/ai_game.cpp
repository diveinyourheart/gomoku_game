#include "ai_game.h"

AIGame::AIGame(bool humanPlaysBlack, QObject *parent) : Game(parent), humanPlayer(nullptr), aiPlayer(nullptr), humanPlaysBlack(humanPlaysBlack)
{
    if (humanPlaysBlack) {
        // 人类玩家作为黑方（索引0），AI作为白方（索引1）
        humanPlayerIndex = 0;
        aiPlayerIndex = 1;
        humanPlayer = new GomokuPlayer(GomokuBoard::BLACK);
        aiPlayer = new AIPlayer(GomokuBoard::WHITE);
    } else {
        // 人类玩家作为白方（索引1），AI作为黑方（索引0）
        humanPlayerIndex = 1;
        aiPlayerIndex = 0;
        humanPlayer = new GomokuPlayer(GomokuBoard::WHITE);
        aiPlayer = new AIPlayer(GomokuBoard::BLACK);
    }
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
    
    // 如果AI执黑子（人类执白子），则AI先下
    if (aiPlayerIndex == 0) {
        // AI自动落子
        Move aiMove = aiPlayer->aiDecision(&board);
        makeMove(aiMove.x, aiMove.y);
    }
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
    // AI模式悔棋较为复杂，需要考虑AI的落子记录和人类玩家的落子记录
    // 这里简单地返回false，实际实现需要根据AI的落子记录和人类玩家的落子记录来判断
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

int AIGame::getHumanPlayerIndex() const
{
    return humanPlayerIndex;
}

int AIGame::getAiPlayerIndex() const
{
    return aiPlayerIndex;
}

bool AIGame::canPlayerMove() const
{
    return gameStatus == GAME_IN_PROGRESS && currentPlayerIndex == humanPlayerIndex;
}