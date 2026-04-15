#ifndef AI_GAME_H
#define AI_GAME_H

#include "game.h"
#include "gomoku_player.h"
#include "ai_player.h"

class AIGame : public Game
{
    Q_OBJECT

public:
    // 回合特征类
    class TurnFeature {
    public:
        int turnNumber;      // 当前回合数
        int playerIndex;     // 当前玩家索引
        PlayerMove move;     // 当前回合玩家落子坐标

        TurnFeature(int turn = 0, int player = 0, const PlayerMove& m = PlayerMove(-1, -1))
            : turnNumber(turn), playerIndex(player), move(m) {}

        bool operator==(const TurnFeature& other) const {
            return turnNumber == other.turnNumber && 
                   playerIndex == other.playerIndex && 
                   move.x == other.move.x && 
                   move.y == other.move.y;
        }

        bool operator!=(const TurnFeature& other) const {
            return !(*this == other);
        }
    };

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
    
    // 获取当前回合特征
    TurnFeature getCurrentTurnFeature() const;

private:
    GomokuPlayer* humanPlayer; // 人类玩家
    AIPlayer* aiPlayer; // AI玩家
    int aiPlayerIndex; // AI玩家的索引（0或1）
    int humanPlayerIndex; // 人类玩家的索引（0或1）
    bool humanPlaysBlack; // 人类玩家是否执黑子
};

#endif // AI_GAME_H