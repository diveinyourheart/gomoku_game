#ifndef NETWORK_EVENT_H
#define NETWORK_EVENT_H

#include <QEvent>
#include "player.h"
#include <vector>
#include <utility>

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

    NetworkResponseEvent(ResponseType type, const PlayerMove& move)
        : QEvent(NETWORK_RESPONSE_EVENT), type(type), move(move) {}

    NetworkResponseEvent(ResponseType type, const std::vector<std::pair<std::pair<int, int>, int>>& scores)
        : QEvent(NETWORK_RESPONSE_EVENT), type(type), scores(scores) {}

    ResponseType getType() const { return type; }
    PlayerMove getMove() const { return move; }
    std::vector<std::pair<std::pair<int, int>, int>> getScores() const { return scores; }

private:
    ResponseType type;
    PlayerMove move; // 用于GET_BEST_MOVE
    std::vector<std::pair<std::pair<int, int>, int>> scores; // 用于GET_MOVE_SCORES
};

#endif // NETWORK_EVENT_H
