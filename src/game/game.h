#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "player.h"
#include "gomoku_board.h"
#include <sstream>

class Game : public QObject
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

        friend QDebug& operator<<(QDebug& dbg, const TurnFeature& feature) {
            dbg << "TurnFeature(turnNumber=" << feature.turnNumber;
            dbg << ", playerIndex=" << feature.playerIndex;
            dbg << ", move=" << feature.move.x << "," << feature.move.y;
            dbg << ")";
            return dbg;
        }

        friend std::ostringstream& operator<<(std::ostringstream& oss, const TurnFeature& feature) {
            oss << "TurnFeature(turnNumber=" << feature.turnNumber;
            oss << ", playerIndex=" << feature.playerIndex;
            oss << ", move=" << feature.move.x << "," << feature.move.y;
            oss << ")";
            return oss;
        }
    };

    static const int GAME_NOT_STARTED = 0;
    static const int GAME_IN_PROGRESS = 1;
    static const int GAME_FINISHED = 2;

    Game(QObject *parent = nullptr);
    virtual ~Game();

    virtual void startNewGame() = 0;
    virtual bool makeMove(int x, int y) = 0;
    virtual bool undoMove() = 0;
    virtual int getCurrentPlayer() const = 0;
    virtual int getGameStatus() const = 0;
    virtual int getWinner() const = 0;
    virtual GomokuBoard* getBoard() = 0;
    virtual int getMoveCount() const = 0;
    virtual bool canPlayerMove() const = 0;
    virtual TurnFeature getCurrentTurnFeature() const = 0;
    bool getHasUndone() const; // 获取悔棋标志

signals:
    void gameOver(int winner); // 0: 平局, 1: 黑方胜, 2: 白方胜
    void moveMade(); // 落子成功后发送信号
    void undoButtonStateChanged(bool enabled); // 悔棋按钮状态变化信号

protected:
    GomokuBoard board;
    int currentPlayerIndex;
    int gameStatus;
    int winner;
    int moveCount; // 回合数
    bool hasUndone; // 表示当前用户已经悔棋的标志
};

#endif // GAME_H