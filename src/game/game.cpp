#include "game.h"

Game::Game(QObject *parent) : QObject(parent), currentPlayerIndex(0), gameStatus(GAME_NOT_STARTED), winner(0), moveCount(0)
{
}

Game::~Game()
{
}

bool Game::canPlayerMove() const
{
    return gameStatus == GAME_IN_PROGRESS;
}