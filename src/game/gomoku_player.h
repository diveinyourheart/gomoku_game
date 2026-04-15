#ifndef GOMOKU_PLAYER_H
#define GOMOKU_PLAYER_H

#include "player.h"

class GomokuPlayer : public Player
{
public:
    GomokuPlayer(int color);
    ~GomokuPlayer();
};

#endif // GOMOKU_PLAYER_H