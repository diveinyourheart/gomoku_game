#include "game.h"

Game::Game(QObject *parent) : QObject(parent), currentPlayerIndex(0), gameStatus(GAME_NOT_STARTED), winner(0), moveCount(0), hasUndone(false)
{
}

Game::~Game()
{
}

bool Game::getHasUndone() const
{
    return hasUndone;
}