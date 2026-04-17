#ifdef MY_DEBUG

#ifndef __GOMOKUGAMELOG_H__
#define __GOMOKUGAMELOG_H__
#include <vector>
#include <memory>
#include <ctime>
#include <chrono>
#include <iomanip>

class LogSink{
    public:
    virtual ~LogSink() = default;
    virtual void write(const char* level, const std::string& message) = 0;
};

class Logger{
public:
    static const std::string delimiter;
    static Logger& instance();
    void addSink(std::shared_ptr<LogSink> sink);
    template<typename... Args>
    void log(const char* level,
             const char* file,
             const char* func,
             int line,
             Args&&... args){
        std::ostringstream oss;
        
        oss << file
            << delimiter << func
            << delimiter << line
            << delimiter;
        
        append(oss, std::forward<Args>(args)...);
        oss << "\n";

        auto msg = oss.str();
        for(auto& sink : sinks){
            sink->write(level, msg);
        }
    }

    static std::string timeNow();

private:
    std::vector<std::shared_ptr<LogSink>> sinks;
    Logger() = default;
    ~Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    template<typename T>
    void append(std::ostringstream& oss, T&& value){
        oss << std::forward<T>(value);
    }

    template<typename T, typename... Args>
    void append(std::ostringstream& oss, T&& value, Args&&... args){
        oss << std::forward<T>(value);
        append(oss, std::forward<Args>(args)...);
    }
};




#define LOG_INFO(...) Logger::instance().log("INFO", __FILE__, __func__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) Logger::instance().log("ERROR", __FILE__, __func__, __LINE__, __VA_ARGS__)
#define LOG_WARNING(...) Logger::instance().log("WARNING", __FILE__, __func__, __LINE__, __VA_ARGS__)
#endif
#else
#define LOG_INFO(...)  ((void)0)
#define LOG_ERROR(...) ((void)0)
#define LOG_WARNING(...) ((void)0)
#endif