#ifndef NETWORK_EVENT_H
#define NETWORK_EVENT_H

#include <QEvent>
#include "player.h"
#include "ai_game.h"
#include <vector>
#include <utility>
#include "game.h"

// 事件类型
const QEvent::Type NETWORK_RESPONSE_EVENT = static_cast<QEvent::Type>(QEvent::User + 1);

// 网络响应事件
class NetworkResponseEvent : public QEvent
{
public:
    enum class ResponseType {
        GET_BEST_MOVE,
        GET_MOVE_SCORES
    };

    NetworkResponseEvent(ResponseType type, const PlayerMove& move, const AIGame::TurnFeature& feature)
        : QEvent(NETWORK_RESPONSE_EVENT), type(type), move(move), turnFeature(feature) {}

    NetworkResponseEvent(ResponseType type, const std::vector<std::pair<std::pair<int, int>, int>>& scores, const AIGame::TurnFeature& feature)
        : QEvent(NETWORK_RESPONSE_EVENT), type(type), scores(scores), turnFeature(feature) {}

    ResponseType getType() const { return type; }
    PlayerMove getMove() const { return move; }
    std::vector<std::pair<std::pair<int, int>, int>> getScores() const { return scores; }
    Game::TurnFeature getTurnFeature() const { return turnFeature; }

private:
    ResponseType type;
    PlayerMove move; // 用于GET_BEST_MOVE
    std::vector<std::pair<std::pair<int, int>, int>> scores; // 用于GET_MOVE_SCORES
    AIGame::TurnFeature turnFeature; // 回合特征，用于验证
};

#endif // NETWORK_EVENT_H
