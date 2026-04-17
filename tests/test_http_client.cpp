#ifdef _WIN32
#include <windows.h>
#endif
#include <iostream>
#include <string>
#include <sstream>
#include <random>

#include "../src/game/gomoku_board.h"
#include "../src/network/network_manager.h"
#include "../src/util/env_util.h"
#include "../src/game/player.h"
#include "../src/util/gomoku_console_log.h"

// 声明测试函数
void testHttpClient(const std::string& api_key);

// 声明测试函数
void testHttpClient(const std::string& api_key);

void testRandomBoardGenerator(GomokuBoard& board) {
    LOG_INFO("Testing GomokuBoard random board generator...");

    board.generateRandomBoard(100);

    LOG_INFO("Generated random board (.=empty, X=black, O=white):");
    LOG_INFO(board.boardToString());

    LOG_INFO("RandomBoardGenerator Test completed!");
}

int main() {

    #ifdef _WIN32
    // 设置控制台输出为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
    #endif

    // 加载环境变量
    EnvUtil::getInstance().loadEnvFile("../../.env");
    std::string api_key = EnvUtil::getInstance().getEnv("DEEPSEEK_API_KEY", "");
    LOG_INFO("DEEPSEEK_API_KEY: ",api_key);

    GomokuBoard board;
    testRandomBoardGenerator(board);
    // testHttpClient(api_key);

    NetworkManager networkManager(api_key);

    // 随机生成当前玩家
    // 随机选择当前玩家颜色
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);
    int currentPlayer = (dis(gen) == 0) ? GomokuBoard::BLACK : GomokuBoard::WHITE;
    
    std::string player_str = (currentPlayer == GomokuBoard::BLACK) ? "BLACK" : "WHITE";
    LOG_INFO("Current player: ",player_str);

    PlayerMove move = networkManager.getBestMove(&board, currentPlayer);
    LOG_INFO("Best move: ",move.x,move.y);

    LOG_INFO("\nPress Enter to exit...");
    std::cin.get();
    return 0;
}

// 实现测试函数
void testHttpClient(const std::string& api_key) {
    std::string url = "https://api.deepseek.com/chat/completions";
    
    std::stringstream ss;
    ss << "{";
    ss << "\"model\": \"deepseek-chat\",";
    ss << "\"messages\": [";
    ss << "{\"role\": \"system\", \"content\": \"You are a helpful assistant.\"},";
    ss << "{\"role\": \"user\", \"content\": \"你是谁\"}";
    ss << "],";
    ss << "\"stream\": false";
    ss << "}";
    
    std::string request_data = ss.str();
    
    // Create NetworkManager instance
    NetworkManager networkManager(api_key);
    std::string response = HttpClient::post(url, request_data, api_key);
    
    LOG_INFO("Response from server:");
    LOG_INFO(response);
    
    LOG_INFO("Test completed!");
}
