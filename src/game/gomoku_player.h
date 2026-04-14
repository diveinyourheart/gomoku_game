#ifndef GOMOKU_PLAYER_H
#define GOMOKU_PLAYER_H

#include "player.h"

class GomokuPlayer : public Player
{
public:
    GomokuPlayer(int color);
    ~GomokuPlayer();

    void addMove(int x, int y) override;
    PlayerMove undoMove();
    bool hasMoves() const;

private:
    std::stack<PlayerMove> moveHistory;
};

#endif // GOMOKU_PLAYER_H