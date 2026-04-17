#ifdef MY_DEBUG

#include "log.h"
#include <sstream>
#include <vector>
#include <memory>
#include <ctime>
#include <chrono>
#include <iomanip>

// 初始化Logger类的静态成员
const std::string Logger::delimiter = "|";

// 实现Logger::instance()静态方法
Logger& Logger::instance(){
    static Logger instance;
    return instance;
}

// 实现Logger::addSink()方法
void Logger::addSink(std::shared_ptr<LogSink> sink){
    sinks.push_back(sink);
}

// 实现Logger::timeNow()静态方法
std::string Logger::timeNow(){
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm{};
    #ifdef _WIN32
    localtime_s(&local_tm, &t);
    #else
    localtime_r(&t, &local_tm);
    #endif
    std::ostringstream oss;
    oss << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// 实现Logger::log()模板方法
// 注意：模板方法必须在头文件中实现，这里只是为了完整性而列出

#endif // MY_DEBUG
