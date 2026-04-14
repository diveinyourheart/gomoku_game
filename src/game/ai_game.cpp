#include "ai_game.h"
#include "network_manager.h"
#include "network_event.h"
#include <set>

AIGame::AIGame(bool humanPlaysBlack, QObject *parent) : Game(parent), humanPlayer(nullptr), aiPlayer(nullptr), humanPlaysBlack(humanPlaysBlack)
{
    // 根据参数设置玩家索引和颜色
    if (humanPlaysBlack) {
        humanPlayerIndex = 0; // 黑方
        aiPlayerIndex = 1;    // 白方
        humanPlayer = new GomokuPlayer(GomokuBoard::BLACK);
        aiPlayer = new AIPlayer(GomokuBoard::WHITE);
    } else {
        humanPlayerIndex = 1; // 白方
        aiPlayerIndex = 0;    // 黑方
        humanPlayer = new GomokuPlayer(GomokuBoard::WHITE);
        aiPlayer = new AIPlayer(GomokuBoard::BLACK);
    }
}

AIGame::~AIGame()
{
    delete humanPlayer;
    delete aiPlayer;
}

bool AIGame::event(QEvent* event) {
    if (event->type() == NETWORK_RESPONSE_EVENT) {
        NetworkResponseEvent* responseEvent = static_cast<NetworkResponseEvent*>(event);
        
        if (responseEvent->getType() == NetworkResponseEvent::ResponseType::GET_MOVE_SCORES) {
            // 处理AI决策结果
            handleAIDecision(responseEvent->getScores());
        }
        
        return true;
    }
    
    return QObject::event(event);
}

void AIGame::handleAIDecision(const std::vector<std::pair<std::pair<int, int>, int>>& scores) {
    if (scores.empty()) {
        return;
    }
    
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
    
    // 如果有重复的位置，使用本地决策
    if (hasDuplicates) {
        PlayerMove aiMove = aiPlayer->localDecision(&board);
        makeMove(aiMove.x, aiMove.y);
        return;
    }
    
    // 找到分数最高的位置
    int maxScore = -1;
    std::pair<int, int> bestMove = scores[0].first;
    
    for (const auto& score : scores) {
        int x = score.first.first;
        int y = score.first.second;
        int s = score.second;
        
        // 检查位置是否有效（棋盘上是否为空）
        if (board.getStone(x, y) == GomokuBoard::EMPTY) {
            if (s > maxScore) {
                maxScore = s;
                bestMove = std::make_pair(x, y);
            }
        }
    }
    
    // 如果找到有效位置，执行落子
    if (maxScore != -1) {
        makeMove(bestMove.first, bestMove.second);
    } else {
        // 如果没有找到有效位置，使用本地决策
        PlayerMove aiMove = aiPlayer->localDecision(&board);
        makeMove(aiMove.x, aiMove.y);
    }
}

void AIGame::startNewGame()
{
    board.reset();
    currentPlayerIndex = 0; // 黑方先下
    gameStatus = GAME_IN_PROGRESS;
    winner = 0;
    moveCount = 0;
    
    // 如果AI执黑子（人类执白子），则AI先下
    if (aiPlayerIndex == 0) {
        // AI自动落子
        PlayerMove aiMove = aiPlayer->aiDecision(&board);
        makeMove(aiMove.x, aiMove.y);
    }
}

bool AIGame::makeMove(int x, int y)
{
    if (gameStatus != GAME_IN_PROGRESS) {
        return false;
    }

    // 只有人类玩家可以手动落子
    if (currentPlayerIndex == humanPlayerIndex) {
        int currentPlayerColor = humanPlayer->getColor();
        if (board.placeStone(x, y, currentPlayerColor)) {
            // 记录人类玩家的落子
            humanPlayer->addMove(x, y);
            
            moveCount++;
            if (board.checkWin(x, y, currentPlayerColor)) {
                gameStatus = GAME_FINISHED;
                winner = currentPlayerColor;
                emit gameOver(winner);
            } else if (board.isFull()) {
                gameStatus = GAME_FINISHED;
                winner = 0; // 平局
                emit gameOver(winner);
            } else {
                currentPlayerIndex = aiPlayerIndex; // 轮到AI
                emit moveMade(); // 发送信号通知UI更新
                
                // 异步获取AI决策
                aiPlayer->asyncDecision(&board, this);
            }
            return true;
        }
    } else if (currentPlayerIndex == aiPlayerIndex) {
        // AI落子（由handleAIDecision方法处理）
        int currentPlayerColor = aiPlayer->getColor();
        if (board.placeStone(x, y, currentPlayerColor)) {
            // AI不需要记录落子
            
            moveCount++;
            if (board.checkWin(x, y, currentPlayerColor)) {
                gameStatus = GAME_FINISHED;
                winner = currentPlayerColor;
                emit gameOver(winner);
            } else if (board.isFull()) {
                gameStatus = GAME_FINISHED;
                winner = 0; // 平局
                emit gameOver(winner);
            } else {
                currentPlayerIndex = humanPlayerIndex; // 轮到人类玩家
                emit moveMade(); // 发送信号通知UI更新
            }
            return true;
        }
    }
    return false;
}

bool AIGame::undoMove()
{
    // AI模式悔棋较为复杂，需要考虑AI的落子记录和人类玩家的落子记录
    // 这里简单地返回false，实际实现需要根据AI的落子记录和人类玩家的落子记录来判断
    return false;
}

int AIGame::getCurrentPlayer() const
{
    return currentPlayerIndex;
}

int AIGame::getGameStatus() const
{
    return gameStatus;
}

int AIGame::getWinner() const
{
    return winner;
}

GomokuBoard* AIGame::getBoard()
{
    return &board;
}

int AIGame::getMoveCount() const
{
    return moveCount;
}

int AIGame::getHumanPlayerIndex() const
{
    return humanPlayerIndex;
}

int AIGame::getAiPlayerIndex() const
{
    return aiPlayerIndex;
}

bool AIGame::canPlayerMove() const
{
    return gameStatus == GAME_IN_PROGRESS && currentPlayerIndex == humanPlayerIndex;
}