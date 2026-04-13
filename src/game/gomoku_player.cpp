#include "gomoku_player.h"

GomokuPlayer::GomokuPlayer(int color) : Player(Player::HUMAN, color)
{
}

GomokuPlayer::~GomokuPlayer()
{
}

void GomokuPlayer::addMove(int x, int y)
{
    moveHistory.push(Move(x, y));
}

Move GomokuPlayer::undoMove()
{
    if (!moveHistory.empty()) {
        Move move = moveHistory.top();
        moveHistory.pop();
        return move;
    }
    return Move(-1, -1);
}

bool GomokuPlayer::hasMoves() const
{
    return !moveHistory.empty();
}