#include "env_util.h"
#include <fstream>
#include <sstream>
#include <iostream>

EnvUtil::EnvUtil() {
    // 构造函数
}

EnvUtil::~EnvUtil() {
    // 析构函数
}

EnvUtil& EnvUtil::getInstance() {
    static EnvUtil instance;
    return instance;
}

bool EnvUtil::loadEnvFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "无法打开环境变量文件: " << filePath << std::endl;
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // 跳过空行和注释
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        // 解析键值对
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            
            // 去除首尾空格
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            
            // 去除引号
            if (!value.empty() && (value[0] == '"' || value[0] == '\'')) {
                value = value.substr(1, value.size() - 2);
            }
            
            envMap[key] = value;
        }
    }
    
    file.close();
    return true;
}

std::string EnvUtil::getEnv(const std::string& key, const std::string& defaultValue) {
    auto it = envMap.find(key);
    if (it != envMap.end()) {
        return it->second;
    }
    return defaultValue;
}

void EnvUtil::setEnv(const std::string& key, const std::string& value) {
    envMap[key] = value;
}
