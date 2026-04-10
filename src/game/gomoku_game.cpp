#include "gomoku_game.h"

GomokuGame::GomokuGame(QObject *parent)
    : QObject(parent), currentPlayerIndex(0), gameStatus(GAME_NOT_STARTED), winner(0), moveCount(0)
{
    players[0] = new GomokuPlayer(GomokuPlayer::HUMAN, GomokuBoard::BLACK);
    players[1] = new GomokuPlayer(GomokuPlayer::HUMAN, GomokuBoard::WHITE);
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

    int currentPlayerColor = players[currentPlayerIndex]->getColor();
    if (board.placeStone(x, y, currentPlayerColor)) {
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
            currentPlayerIndex = (currentPlayerIndex + 1) % 2;
            emit moveMade(); // 落子成功且游戏未结束，发送信号
        }
        return true;
    }
    return false;
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