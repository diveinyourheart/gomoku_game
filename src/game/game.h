#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "player.h"
#include "gomoku_board.h"

class Game : public QObject
{
    Q_OBJECT

public:
    static const int GAME_NOT_STARTED = 0;
    static const int GAME_IN_PROGRESS = 1;
    static const int GAME_FINISHED = 2;

    Game(QObject *parent = nullptr);
    virtual ~Game();

    virtual void startNewGame() = 0;
    virtual bool makeMove(int x, int y) = 0;
    virtual bool undoMove() = 0;
    virtual int getCurrentPlayer() const = 0;
    virtual int getGameStatus() const = 0;
    virtual int getWinner() const = 0;
    virtual GomokuBoard* getBoard() = 0;
    virtual int getMoveCount() const = 0;

signals:
    void gameOver(int winner); // 0: 平局, 1: 黑方胜, 2: 白方胜
    void moveMade(); // 落子成功后发送信号

protected:
    GomokuBoard board;
    int currentPlayerIndex;
    int gameStatus;
    int winner;
    int moveCount; // 回合数
};

#endif // GAME_H