#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <string>
#include <memory>
#include <vector>
#include <utility>
#include "gomoku_board.h"

struct Move;
class HttpClient {
public:
    static std::string post(const std::string& url, const std::string& request_data, const std::string& api_key);
};

class NetworkManager {
public:
    NetworkManager(const std::string& api_key);
    ~NetworkManager();

    // 发送棋盘状态给DeepSeek，获取最佳落子
    Move getBestMove(const GomokuBoard* board, int currentPlayer);
    
    // 发送候选点给DeepSeek，获取评分
    std::vector<std::pair<std::pair<int, int>, int>> getMoveScores(const GomokuBoard* board, int currentPlayer, const std::vector<std::pair<int, int>>& candidates, int topK);

private:
    std::string api_key;
    std::string api_url = "https://api.deepseek.com/v1/chat/completions";

    // 将棋盘状态转换为字符串
    std::string boardToString(const GomokuBoard* board);
    
    // 发送HTTP请求
    std::string sendRequest(const std::string& system_content, const std::string& user_content);
    
    // 解析响应，提取坐标
    Move parseResponse(const std::string& response);
    
    // 解析响应，提取评分
    std::vector<std::pair<std::pair<int, int>, int>> parseScoreResponse(const std::string& response);
};

#endif // NETWORK_MANAGER_H
