#include "config.h"
#include "env_util.h"

const std::string API_KEY = []() {
    EnvUtil::getInstance().loadEnvFile("../.env");
    return EnvUtil::getInstance().getEnv("DEEPSEEK_API_KEY", "");
}();