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
    // 基类空实现，由子类重写
}