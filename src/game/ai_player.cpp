#include "ai_player.h"
#include "network_manager.h"
#include "config.h"
#include <vector>
#include <algorithm>
#include <random>
#include <utility>
#include <set>
#include <cmath>

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
    // 获取棋盘上的所有有效位置
    std::vector<std::pair<int, int>> validMoves = board->getValidMoves();
    if (validMoves.empty()) {
        return Move(-1, -1); // 棋盘已满
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
                return Move(bestMove.first, bestMove.second);
            }
        }
    }
    
    // 兜底策略：从本地评分最高的候选点中选择
    if (!localScores.empty()) {
        return Move(localScores[0].first.first, localScores[0].first.second);
    }
    
    // 如果本地评分也没有结果，随机选择一个
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, validMoves.size() - 1);
    int randomIndex = dist(gen);
    return Move(validMoves[randomIndex].first, validMoves[randomIndex].second);
}



int AIPlayer::evaluateBoard(const GomokuBoard* board)
{
    // 简单的棋盘评估函数
    // 后续可以实现更复杂的评估逻辑
    return 0;
}