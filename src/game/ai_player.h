#ifndef AI_PLAYER_H
#define AI_PLAYER_H

#include <memory>
#include <stack>
#include <vector>
#include <utility>
#include "player.h"
#include "gomoku_board.h"
class NetworkManager;

class AIPlayer : public Player
{
public:
    AIPlayer(int color);
    ~AIPlayer();

    // AI决策方法
    PlayerMove aiDecision(const GomokuBoard* board);
    // 本地决策方法（使用minimax算法）
    PlayerMove localDecision(const GomokuBoard* board, int depth = 3);
    // 异步决策方法
    void asyncDecision(const GomokuBoard* board, class AIGame* game);

private:
    std::unique_ptr<NetworkManager> networkManager;
    
    // AI相关的辅助方法
    int evaluateBoard(const GomokuBoard* board); // 评估棋盘状态
    int minimax(GomokuBoard* board, int depth, int alpha, int beta, bool maximizingPlayer, int aiPlayer, int opponentPlayer);
    std::vector<std::pair<int, int>> generateMoves(const GomokuBoard* board);
    
public:
    // 获取NetworkManager实例
    NetworkManager* getNetworkManager() const;

};

#endif // AI_PLAYER_H