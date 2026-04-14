#include "network_manager.h"
#include "httplib.h"
#include <iostream>
#include <string>
#include <sstream>
#include "player.h"
#include <nlohmann/json.hpp> 
#ifdef QT_CORE_LIB
#include <QDebug>
#include <QString>
#endif

// 使用cpp-httplib库实现HTTP客户端
std::string HttpClient::post(const std::string& url, const std::string& data, const std::string& api_key) {
    // 解析URL
    size_t pos = url.find("://");
    if (pos == std::string::npos) {
        return "";
    }
    std::string scheme = url.substr(0, pos);
    std::string host_and_path = url.substr(pos + 3);
    pos = host_and_path.find("/");
    std::string host = host_and_path.substr(0, pos);
    std::string path = host_and_path.substr(pos);
    
    std::cout << "host: " << host << std::endl;
    std::cout << "path: " << path << std::endl;
    std::cout << "data: " << std::endl;
    std::cout << data << std::endl;
    
    // 创建客户端
    httplib::SSLClient client(host.c_str());
    // client.set_ca_cert_path("./cacert.pem");
    client.enable_server_certificate_verification(false);
    client.set_connection_timeout(30, 0); // 设置连接超时为30秒
    
    // 设置请求头
    httplib::Headers headers = {
        {"Content-Type", "application/json"},
        {"Authorization", "Bearer " + api_key}
    };
    
    // 发送请求
    auto res = client.Post(path.c_str(), headers, data, "application/json");

    if (!res) {
        std::cerr << "SSLClient.Post方法的返回值为空" << std::endl;
        return "";
    }else{
        std::cout << "response: " << std::endl;
        std::cout << "status: " << res->status << std::endl;
        std::cout << "body: " << res->body << std::endl;
    }

    #ifdef QT_CORE_LIB
    if (res) {
        qDebug() << "[HTTP Response] Status:" << res->status;
        qDebug() << "[HTTP Response] Body:" << QString::fromStdString(res->body);
    } else {
        qDebug() << "[HTTP Response] Error: Response is null (Connection failed)";
    }
    #endif

    if (res && res->status == 200) {
        return res->body;
    } else {
        if (res) {
            std::cerr << "HTTP请求失败: " << res->status << std::endl;
        } else {
            std::cerr << "HTTP请求失败: 连接错误" << std::endl;
        }
        return "";
    }
}

NetworkManager::NetworkManager(const std::string& api_key) : api_key(api_key) {}

NetworkManager::~NetworkManager() {}

std::string NetworkManager::boardToString(const GomokuBoard* board) {
    std::stringstream ss;
    for (int y = 0; y < GomokuBoard::BOARD_SIZE; ++y) {
        for (int x = 0; x < GomokuBoard::BOARD_SIZE; ++x) {
            int stone = board->getStone(x, y);
            switch (stone) {
                case GomokuBoard::EMPTY: ss << ".";
                    break;
                case GomokuBoard::BLACK: ss << "X";
                    break;
                case GomokuBoard::WHITE: ss << "O";
                    break;
            }
        }
        ss << "\n";
    }
    return ss.str();
}

std::string NetworkManager::sendRequest(const std::string& system_content, const std::string& user_content) {
    // 1. 创建 json 对象
    nlohmann::json j;

    // 2. 填充字段
    j["model"] = "deepseek-chat";
    j["messages"] = nlohmann::json::array({
        {
            {"role", "system"}, 
            {"content", system_content}
        },
        {
            {"role", "user"}, 
            {"content", user_content} // 这里的 prompt 即使包含换行符，dump 时也会被自动处理
        }
    });
    j["stream"] = false;
    j["temperature"] = 0.1;
    j["max_tokens"] = 100;

    // 3. 序列化为字符串 (dump 自动处理所有字符转义)
    std::string request_data = j.dump();

    // 4. 发送请求
    return HttpClient::post(api_url, request_data, api_key);
}

/*

大模型api返回json格式：
{
  "id": "30294237-7590-45dc-b487-31cef26b9281",
  "object": "chat.completion",
  "created": 1776074295,
  "model": "deepseek-chat",
  "choices": [
    {
      "index": 0,
      "message": {
        "role": "assistant",
        "content": "我是DeepSeek，由深度求索公司创造的AI助手！😊\n\n我是一个纯文本模型，虽然不支持多模态识别功能，但我有文件上传功能，可以处理图像、txt、pdf、ppt、word、excel等文件，并从中读取文字信息来帮助你。我完全免费使用，拥有128K的上下文长度，还支持联网搜索功能（需要你在Web/App中手动点开联网搜索按键）。\n\n你可以通过官方应用商店下载我的App来使用。我很乐意为你解答问题、协助处理文档、进行对话交流等等！\n\n有什么我可以帮助你的吗？无论是学习、工作还是日常问题，我都很愿意为你提供帮助！✨"
      },
      "logprobs": null,
      "finish_reason": "stop"
    }
  ],
  "usage": {
    "prompt_tokens": 11,
    "completion_tokens": 139,
    "total_tokens": 150,
    "prompt_tokens_details": {
      "cached_tokens": 0
    },
    "prompt_cache_hit_tokens": 0,
    "prompt_cache_miss_tokens": 11
  },
  "system_fingerprint": "fp_eaab8d114b_prod0820_fp8_kvcache_new_kvcache_20260410"
}

*/

Move NetworkManager::parseResponse(const std::string& response) {
    try {
        auto json = nlohmann::json::parse(response);
        std::string content = json["choices"][0]["message"]["content"];
        
        // 假设 AI 返回 "10,15" 格式
        size_t comma = content.find(",");
        if (comma != std::string::npos) {
            int x = std::stoi(content.substr(0, comma));
            int y = std::stoi(content.substr(comma + 1));
            
            if (x >= 0 && x < GomokuBoard::BOARD_SIZE &&
                y >= 0 && y < GomokuBoard::BOARD_SIZE) {
                return Move(x, y);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "JSON 解析失败: " << e.what() << std::endl;
    }
    
    // 解析失败时的降级策略
    return Move(-1, -1);  // 返回特殊值表示解析失败
}

Move NetworkManager::getBestMove(const GomokuBoard* board, int currentPlayer) {
    std::string board_str = boardToString(board);
    std::string player_str = (currentPlayer == GomokuBoard::BLACK) ? "X" : "O";
    
    // 1. 系统指令：建立严格规则
    std::string system_content = 
        "你是一个五子棋AI。你的任务是分析棋盘并给出下一个落子点。\n"
        "【严格规则】：\n"
        "1. 只输出坐标，格式为 x,y（例如：7,7）。\n"
        "2. 严禁输出任何文字说明、思考过程或多余字符。\n"
        "3. 即使棋局结束或有其他情况，也只准输出坐标。\n"
        "4. 输出坐标时，坐标对应的棋盘位置必须不能是'X'或'O'，必须是'.'。\n";

    // 2. 用户指令：提供数据并再次强调
    std::string user_content = 
        "棋盘状态：\n" + board_str + "\n\n" +
        "当前轮到: " + player_str + "\n" +
        "请直接输出最佳落子坐标，严禁任何废话。";
    
    std::string response = sendRequest(system_content, user_content);
    return parseResponse(response);
}

std::vector<std::pair<std::pair<int, int>, int>> NetworkManager::getMoveScores(const GomokuBoard* board, int currentPlayer, const std::vector<std::pair<std::pair<int, int>, int>>& candidatesWithScores, int topK) {
    std::string board_str = boardToString(board);
    std::string player_str = (currentPlayer == GomokuBoard::BLACK) ? "X" : "O";
    
    // 构建候选点JSON串（包含评估分数）
    nlohmann::json candidates_json = nlohmann::json::array();
    for (const auto& candidate : candidatesWithScores) {
        nlohmann::json move;
        move["x"] = candidate.first.first;
        move["y"] = candidate.first.second;
        move["score"] = candidate.second;
        candidates_json.push_back(move);
    }
    std::string candidates_str = candidates_json.dump();
    
    // 系统指令
    std::string system_content =
        "你是一个五子棋AI。\n"
        "请从候选落子点中选择最优的点并重新打分（0-100）。\n"
        "\n"
        "【评分构成】\n"
        "- 进攻分：评估当前落子对自己进攻的帮助\n"
        "- 防守分：评估当前落子对阻止对手进攻的帮助\n"
        "- 总评分 = 进攻分 × 1.0 + 防守分 × 1.2\n"
        "\n"
        "【评分规则参考】\n"
        "- 连五（FIVE）: 1000000\n"
        "- 活四（OPEN_FOUR）: 100000\n"
        "- 冲四（FOUR）: 10000\n"
        "- 活三（OPEN_THREE）: 1000\n"
        "- 眠三（THREE）: 200\n"
        "- 双活二（TWO）: 50\n"
        "- 单活二（ONE）: 10\n"
        "\n"
        "【严格规则】\n"
        "1. 只能从提供的候选列表中选择坐标\n"
        "2. 输出必须是合法 JSON 数组\n"
        "3. 不能包含任何额外文字或解释\n"
        "4. 不要使用 markdown 或 ```\n"
        "5. 每个元素格式必须为 {\"x\":int,\"y\":int,\"score\":int}\n"
        "6. 必须返回且只返回 " + std::to_string(topK) + " 个元素\n"
        "7. 结果必须按 score 从高到低排序\n"
        "8. 如果候选点数量少于 " + std::to_string(topK) + "，则返回所有候选点\n"
        "9. 返回的坐标不能重复\n"
        "10. score 必须是 0 到 100 的整数\n"
        "11. x 和 y 必须是整数，且必须与候选列表完全一致\n"
        "12. 分数必须体现差异，不能全部相同\n"
        "13. 请参考提供的评估分数，但可以根据你的判断进行调整\n";

    // 用户指令
    std::string user_content =
        "棋盘：\n" + board_str + "\n\n"
        "候选点（包含评估分数，JSON）：\n" + candidates_str + "\n\n"
        "请返回重新评估后的结果。";
    
    std::string response = sendRequest(system_content, user_content);
    return parseScoreResponse(response);
}

std::vector<std::pair<std::pair<int, int>, int>> NetworkManager::parseScoreResponse(const std::string& response) {
    std::vector<std::pair<std::pair<int, int>, int>> scores;
    
    try {
        auto json = nlohmann::json::parse(response);
        std::string content = json["choices"][0]["message"]["content"];
        
        // 解析JSON数组
        auto score_array = nlohmann::json::parse(content);
        for (const auto& item : score_array) {
            int x = item["x"];
            int y = item["y"];
            int score = item["score"];
            scores.push_back(std::make_pair(std::make_pair(x, y), score));
        }
    } catch (const std::exception& e) {
        std::cerr << "JSON 解析失败: " << e.what() << std::endl;
    }
    
    return scores;
}
