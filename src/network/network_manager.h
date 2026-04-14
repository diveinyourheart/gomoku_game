#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <string>
#include <vector>
#include <utility>
#include <thread>
#include <functional>
#include "player.h"
#include <QCoreApplication>
#include "cameron314/blockingconcurrentqueue.h"

class GomokuBoard;
class AIGame;

// 请求类型
enum class RequestType {
    GET_BEST_MOVE,
    GET_MOVE_SCORES
};

// 请求数据结构
struct NetworkRequest {
    RequestType type;
    std::string system_content;
    std::string user_content;
    int currentPlayer;
    AIGame* game;
    // 用于GET_MOVE_SCORES
    std::vector<std::pair<std::pair<int, int>, int>> candidatesWithScores;
    int topK;
};

class HttpClient {
public:
    static std::string post(const std::string& url, const std::string& data, const std::string& api_key);
};

class NetworkManager {
public:
    NetworkManager(const std::string& api_key);
    ~NetworkManager();
    
    // 发送棋盘状态给DeepSeek，获取最佳落子
    PlayerMove getBestMove(const GomokuBoard* board, int currentPlayer);
    
    // 发送候选点给DeepSeek，获取评分
    std::vector<std::pair<std::pair<int, int>, int>> getMoveScores(const GomokuBoard* board, int currentPlayer, const std::vector<std::pair<std::pair<int, int>, int>>& candidatesWithScores, int topK);
    
    // 异步发送请求
    void asyncGetMoveScores(const GomokuBoard* board, int currentPlayer, const std::vector<std::pair<std::pair<int, int>, int>>& candidatesWithScores, int topK, AIGame* game);

private:
    std::string api_key;
    std::string api_url = "https://api.deepseek.com/v1/chat/completions";
    std::thread networkThread;
    moodycamel::BlockingConcurrentQueue<NetworkRequest> requestQueue;
    bool running;
    
    // 辅助方法
    std::string boardToString(const GomokuBoard* board);
    std::string sendRequest(const std::string& system_content, const std::string& user_content);
    PlayerMove parseResponse(const std::string& response);
    std::vector<std::pair<std::pair<int, int>, int>> parseScoreResponse(const std::string& response);
    
    // 线程方法
    void networkThreadFunc();
    void processRequest(const NetworkRequest& request);
};

#endif // NETWORK_MANAGER_H
