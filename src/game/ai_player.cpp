#include "ai_player.h"
#include "network_manager.h"
#include "config.h"
#include <vector>
#include <algorithm>
#include <utility>
#include <set>
#include <climits>

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

PlayerMove AIPlayer::aiDecision(const GomokuBoard* board)
{
    // 获取棋盘上的所有有效位置
    std::vector<std::pair<int, int>> validMoves = board->getValidMoves();
    if (validMoves.empty()) {
        return PlayerMove(-1, -1); // 棋盘已满
    }
    
    // 定义topK值
    int topK = 5;
    int localTopK = 15; // 本地筛选的候选点数量
    
    // 对所有有效位置进行本地评分
    std::vector<std::pair<std::pair<int, int>, int>> localScores;
    for (const auto& move : validMoves) {
        int x = move.first;
        int y = move.second;
        int score = board->evaluateMove(x, y, getColor());
        if (score > 0) {
            localScores.emplace_back(move, score);
        }
    }
    
    // 按照本地评分排序
    std::sort(localScores.begin(), localScores.end(), 
              [](const auto& a, const auto& b) { 
                  return a.second > b.second; 
              });
    
    // 选择前localTopK个分数最高的候选点
    std::vector<std::pair<std::pair<int, int>, int>> candidatesWithScores;
    int candidateCount = std::min(localTopK, static_cast<int>(localScores.size()));
    for (int i = 0; i < candidateCount; ++i) {
        candidatesWithScores.push_back(localScores[i]);
    }
    
    // 如果没有候选点，使用所有有效位置并计算分数
    if (candidatesWithScores.empty()) {
        for (const auto& move : validMoves) {
            int x = move.first;
            int y = move.second;
            int score = board->evaluateMove(x, y, getColor());
            candidatesWithScores.emplace_back(move, score);
        }
    }
    
    // 最多重试3次
    int maxRetries = 3;
    for (int i = 0; i < maxRetries; ++i) {
        // 获取AI对筛选后候选点的评分
        std::vector<std::pair<std::pair<int, int>, int>> scores = networkManager->getMoveScores(board, getColor(), candidatesWithScores, topK);
        
        // 检查评分结果是否有效
        if (!scores.empty()) {
            // 检查是否有重复的位置
            std::set<std::pair<int, int>> seenMoves;
            bool hasDuplicates = false;
            
            for (const auto& score : scores) {
                int x = score.first.first;
                int y = score.first.second;
                std::pair<int, int> currentMove = std::make_pair(x, y);
                
                if (seenMoves.find(currentMove) != seenMoves.end()) {
                    hasDuplicates = true;
                    break;
                }
                seenMoves.insert(currentMove);
            }
            
            // 如果有重复的位置，说明AI没有按照要求，进行重试
            if (hasDuplicates) {
                continue;
            }
            
            // 找到分数最高的位置
            int maxScore = -1;
            std::pair<int, int> bestMove = scores[0].first;
            
            for (const auto& score : scores) {
                int x = score.first.first;
                int y = score.first.second;
                int s = score.second;
                
                // 检查位置是否有效（棋盘上是否为空）
                if (board->getStone(x, y) == GomokuBoard::EMPTY) {
                    if (s > maxScore) {
                        maxScore = s;
                        bestMove = std::make_pair(x, y);
                    }
                }
            }
            
            // 如果找到有效位置，返回
            if (maxScore != -1) {
                return PlayerMove(bestMove.first, bestMove.second);
            }
        }
    }
    
    // 兜底策略：使用本地决策方法
    return localDecision(board, 3);
}



int AIPlayer::evaluateBoard(const GomokuBoard* board)
{
    // 简单的棋盘评估函数，使用GomokuBoard的evaluateMove方法
    int score = 0;
    int aiColor = getColor();
    int opponentColor = (aiColor == GomokuBoard::BLACK) ? GomokuBoard::WHITE : GomokuBoard::BLACK;
    
    // 对所有有效位置进行评估，取最高分数
    std::vector<std::pair<int, int>> validMoves = board->getValidMoves();
    for (const auto& move : validMoves) {
        int x = move.first;
        int y = move.second;
        int moveScore = board->evaluateMove(x, y, aiColor);
        if (moveScore > score) {
            score = moveScore;
        }
    }
    
    return score;
}

NetworkManager* AIPlayer::getNetworkManager() const {
    return networkManager.get();
}

std::vector<std::pair<int, int>> AIPlayer::generateMoves(const GomokuBoard* board)
{
    // 生成所有有效移动
    return board->getValidMoves();
}

int AIPlayer::minimax(GomokuBoard* board, int depth, int alpha, int beta, bool maximizingPlayer, int aiPlayer, int opponentPlayer)
{
    if (depth == 0) {
        return evaluateBoard(board);
    }

    std::vector<std::pair<int, int>> moves = generateMoves(board);

    if (maximizingPlayer) {
        int maxEval = INT_MIN;
        for (const auto& move : moves) {
            int x = move.first;
            int y = move.second;
            
            // 模拟落子
            board->placeStone(x, y, aiPlayer);
            int eval = minimax(board, depth - 1, alpha, beta, false, aiPlayer, opponentPlayer);
            // 撤销落子
            board->removeStone(x, y);
            
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                break; // 剪枝
            }
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for (const auto& move : moves) {
            int x = move.first;
            int y = move.second;
            
            // 模拟落子
            board->placeStone(x, y, opponentPlayer);
            int eval = minimax(board, depth - 1, alpha, beta, true, aiPlayer, opponentPlayer);
            // 撤销落子
            board->removeStone(x, y);
            
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                break; // 剪枝
            }
        }
        return minEval;
    }
}

PlayerMove AIPlayer::localDecision(const GomokuBoard* board, int depth)
{
    // 获取所有有效移动
    std::vector<std::pair<int, int>> moves = generateMoves(board);
    if (moves.empty()) {
        return PlayerMove(-1, -1); // 棋盘已满
    }
    
    int aiColor = getColor();
    int opponentColor = (aiColor == GomokuBoard::BLACK) ? GomokuBoard::WHITE : GomokuBoard::BLACK;
    
    int bestScore = INT_MIN;
    std::pair<int, int> bestMove = moves[0];
    
    // 复制棋盘以进行模拟
    GomokuBoard* tempBoard = new GomokuBoard();
    // 复制原始棋盘状态
    for (int i = 0; i < GomokuBoard::BOARD_SIZE; ++i) {
        for (int j = 0; j < GomokuBoard::BOARD_SIZE; ++j) {
            int stone = board->getStone(i, j);
            if (stone != GomokuBoard::EMPTY) {
                tempBoard->placeStone(i, j, stone);
            }
        }
    }
    
    for (const auto& move : moves) {
        int x = move.first;
        int y = move.second;
        
        // 模拟落子
        tempBoard->placeStone(x, y, aiColor);
        int score = minimax(tempBoard, depth - 1, INT_MIN, INT_MAX, false, aiColor, opponentColor);
        // 撤销落子
        tempBoard->removeStone(x, y);
        
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }
    
    delete tempBoard;
    return PlayerMove(bestMove.first, bestMove.second);
}

void AIPlayer::asyncDecision(const GomokuBoard* board, AIGame* game) {
    // 获取棋盘上的所有有效位置
    std::vector<std::pair<int, int>> validMoves = board->getValidMoves();
    if (validMoves.empty()) {
        return; // 棋盘已满
    }
    
    // 定义topK值
    int topK = 5;
    int localTopK = 15; // 本地筛选的候选点数量
    
    // 对所有有效位置进行本地评分
    std::vector<std::pair<std::pair<int, int>, int>> localScores;
    for (const auto& move : validMoves) {
        int x = move.first;
        int y = move.second;
        int score = board->evaluateMove(x, y, getColor());
        if (score > 0) {
            localScores.emplace_back(move, score);
        }
    }
    
    // 按照本地评分排序
    std::sort(localScores.begin(), localScores.end(), 
              [](const auto& a, const auto& b) { 
                  return a.second > b.second; 
              });
    
    // 选择前localTopK个分数最高的候选点
    std::vector<std::pair<std::pair<int, int>, int>> candidatesWithScores;
    int candidateCount = std::min(localTopK, static_cast<int>(localScores.size()));
    for (int i = 0; i < candidateCount; ++i) {
        candidatesWithScores.push_back(localScores[i]);
    }
    
    // 如果没有候选点，使用所有有效位置并计算分数
    if (candidatesWithScores.empty()) {
        for (const auto& move : validMoves) {
            int x = move.first;
            int y = move.second;
            int score = board->evaluateMove(x, y, getColor());
            candidatesWithScores.emplace_back(move, score);
        }
    }
    
    // 异步获取AI对筛选后候选点的评分
    networkManager->asyncGetMoveScores(board, getColor(), candidatesWithScores, topK, game);
}