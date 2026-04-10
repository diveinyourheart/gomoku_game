#include "gomoku_player.h"

GomokuPlayer::GomokuPlayer(int type, int color)
    : type(type), color(color)
{
}

GomokuPlayer::~GomokuPlayer()
{
}

int GomokuPlayer::getType() const
{
    return type;
}

int GomokuPlayer::getColor() const
{
    return color;
}