#include "ai_player.h"
#include "network_manager.h"
#include "config.h"

AIPlayer::AIPlayer(int color) 
    : Player(Player::AI, color)
    , networkManager(std::make_unique<NetworkManager>(API_KEY))
{
}

AIPlayer::~AIPlayer()
{}

void AIPlayer::addMove(int x, int y)
{
    // AI玩家不需要记录落子历史
}

Move AIPlayer::aiDecision(const GomokuBoard* board)
{
    // 使用DeepSeek获取最佳落子
    return networkManager->getBestMove(board, getColor());
}

int AIPlayer::evaluateBoard(const GomokuBoard* board)
{
    // 简单的棋盘评估函数
    // 后续可以实现更复杂的评估逻辑
    return 0;
}