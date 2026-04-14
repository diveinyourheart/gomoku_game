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
    // 本地决策方法（使用minimax算法）
    Move localDecision(const GomokuBoard* board, int depth = 3);

private:
    std::unique_ptr<NetworkManager> networkManager;
    // AI相关的辅助方法
    int evaluateBoard(const GomokuBoard* board); // 评估棋盘状态
    int minimax(GomokuBoard* board, int depth, int alpha, int beta, bool maximizingPlayer, int aiPlayer, int opponentPlayer);
    std::vector<std::pair<int, int>> generateMoves(const GomokuBoard* board);
};

#endif // AI_PLAYER_H