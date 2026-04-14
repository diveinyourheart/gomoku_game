#ifndef ENV_UTIL_H
#define ENV_UTIL_H

#include <string>
#include <map>

class EnvUtil {
public:
    static EnvUtil& getInstance();
    
    // 加载环境变量文件
    bool loadEnvFile(const std::string& filePath);
    
    // 获取环境变量值
    std::string getEnv(const std::string& key, const std::string& defaultValue = "");
    
    // 设置环境变量值
    void setEnv(const std::string& key, const std::string& value);
    
private:
    EnvUtil();
    ~EnvUtil();
    
    std::map<std::string, std::string> envMap;
};

#endif // ENV_UTIL_H
