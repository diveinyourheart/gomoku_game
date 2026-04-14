#ifndef AI_GAME_H
#define AI_GAME_H

#include "game.h"
#include "gomoku_player.h"
#include "ai_player.h"

class AIGame : public Game
{
    Q_OBJECT

public:
    AIGame(bool humanPlaysBlack = true, QObject *parent = nullptr);
    ~AIGame();

    void startNewGame() override;
    bool makeMove(int x, int y) override;
    bool undoMove() override;
    int getCurrentPlayer() const override;
    int getGameStatus() const override;
    int getWinner() const override;
    GomokuBoard* getBoard() override;
    int getMoveCount() const override;
    int getHumanPlayerIndex() const;
    int getAiPlayerIndex() const;
    bool canPlayerMove() const override;
    
    // 处理AI决策结果
    void handleAIDecision(const std::vector<std::pair<std::pair<int, int>, int>>& scores);
    
    // 处理Qt事件
    bool event(QEvent* event) override;

private:
    GomokuPlayer* humanPlayer; // 人类玩家
    AIPlayer* aiPlayer; // AI玩家
    int aiPlayerIndex; // AI玩家的索引（0或1）
    int humanPlayerIndex; // 人类玩家的索引（0或1）
    bool humanPlaysBlack; // 人类玩家是否执黑子
};

#endif // AI_GAME_H