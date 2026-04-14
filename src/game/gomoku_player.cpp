#include "gomoku_player.h"

GomokuPlayer::GomokuPlayer(int color) : Player(Player::HUMAN, color)
{
}

GomokuPlayer::~GomokuPlayer()
{
}

void GomokuPlayer::addMove(int x, int y)
{
    moveHistory.push(PlayerMove(x, y));
}

PlayerMove GomokuPlayer::undoMove()
{
    if (!moveHistory.empty()) {
        PlayerMove move = moveHistory.top();
        moveHistory.pop();
        return move;
    }
    return PlayerMove(-1, -1);
}

bool GomokuPlayer::hasMoves() const
{
    return !moveHistory.empty();
}