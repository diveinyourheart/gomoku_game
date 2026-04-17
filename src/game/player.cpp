#include "player.h"

Player::Player(int type, int color) : type(type), color(color)
{}

Player::~Player()
{}

int Player::getType() const
{
    return type;
}

int Player::getColor() const
{
    return color;
}

void Player::addMove(int x, int y)
{
    moveHistory.emplace(x, y);
}

PlayerMove Player::undoMove()
{
    if (moveHistory.empty()) return PlayerMove(-1, -1);
    PlayerMove last = moveHistory.top();
    moveHistory.pop();
    return last;
}

bool Player::hasMoves() const
{
    return !moveHistory.empty();
}

const std::stack<PlayerMove>& Player::getMoveHistory() const
{
    return moveHistory;
}

PlayerMove Player::getLastMove() const
{
    return moveHistory.empty() ? PlayerMove(-1, -1) : moveHistory.top();
}

bool PlayerMove::operator==(const PlayerMove& other) const
{
    return x == other.x && y == other.y;
}