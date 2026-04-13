#ifndef AI_PLAYER_H
#define AI_PLAYER_H

#include "player.h"
#include "gomoku_board.h"
#include <memory>
class NetworkManager;

class AIPlayer : public Player
{
public:
    AIPlayer(int color);
    ~AIPlayer();

    // 重写添加落子方法
    void addMove(int x, int y) override;

    // AI决策方法
    Move aiDecision(const GomokuBoard* board);

private:
    std::unique_ptr<NetworkManager> networkManager;
    // AI相关的辅助方法
    int evaluateBoard(const GomokuBoard* board); // 评估棋盘状态
};

#endif // AI_PLAYER_H