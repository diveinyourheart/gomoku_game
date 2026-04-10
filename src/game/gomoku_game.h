#ifndef GOMOKU_GAME_H
#define GOMOKU_GAME_H

#include <QObject>
#include "gomoku_board.h"
#include "gomoku_player.h"

class GomokuGame : public QObject
{
    Q_OBJECT

public:
    static const int GAME_NOT_STARTED = 0;
    static const int GAME_IN_PROGRESS = 1;
    static const int GAME_FINISHED = 2;

    GomokuGame(QObject *parent = nullptr);
    ~GomokuGame();

    void startNewGame();
    bool makeMove(int x, int y);
    int getCurrentPlayer() const;
    int getGameStatus() const;
    int getWinner() const;
    GomokuBoard* getBoard();

signals:
    void gameOver(int winner); // 0: 平局, 1: 黑方胜, 2: 白方胜
    void moveMade(); // 落子成功后发送信号

private:
    GomokuBoard board;
    GomokuPlayer* players[2];
    int currentPlayerIndex;
    int gameStatus;
    int winner;
    int moveCount; // 回合数

public:
    int getMoveCount() const;
};

#endif // GOMOKU_GAME_H