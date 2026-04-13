#ifndef GOMOKU_GAME_H
#define GOMOKU_GAME_H

#include "game.h"
#include "gomoku_player.h"

class GomokuGame : public Game
{
    Q_OBJECT

public:
    GomokuGame(QObject *parent = nullptr);
    ~GomokuGame();

    void startNewGame() override;
    bool makeMove(int x, int y) override;
    bool undoMove() override;
    int getCurrentPlayer() const override;
    int getGameStatus() const override;
    int getWinner() const override;
    GomokuBoard* getBoard() override;
    int getMoveCount() const override;

private:
    GomokuPlayer* players[2];
};

#endif // GOMOKU_GAME_H