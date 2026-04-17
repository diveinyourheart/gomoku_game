#ifdef MY_DEBUG
#ifndef __GOMOKUGAMECONSOLELOG_H__
#define __GOMOKUGAMECONSOLELOG_H__

#include "log.h"
#include <iostream>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#endif

// 统一定义颜色，非 Windows 也可以用
#define COLOR_RED     "\033[31m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_RESET   "\033[0m"

class ConsoleLogSink : public LogSink {
public:
    explicit ConsoleLogSink(std::ostream& out = std::cerr) : out(out) {
#ifdef _WIN32
        // 根据传入的 stream 自动选择句柄
        DWORD nHandle = (&out == &std::cout) ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE;
        HANDLE hOut = GetStdHandle(nHandle);
        
        if (hOut != INVALID_HANDLE_VALUE) {
            DWORD dwMode = 0;
            if (GetConsoleMode(hOut, &dwMode)) {
                dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(hOut, dwMode);
            }
        }
#endif
    }

    void write(const char* level, const std::string& message) override {
        const char* color = COLOR_RESET;

        // 使用 strcmp 需要确保 level 字符串内容完全一致
        if (std::strcmp(level, "INFO") == 0) {
            color = COLOR_GREEN;
        } else if (std::strcmp(level, "WARNING") == 0) {
            color = COLOR_YELLOW;
        } else if (std::strcmp(level, "ERROR") == 0) {
            color = COLOR_RED;
        }

        // 统一输出格式，减少 #ifdef 冗余
        out << Logger::timeNow() << Logger::delimiter
            << color << level << COLOR_RESET << Logger::delimiter
            << message << std::flush; // 强制刷新，确保实时看到
    }

private:
    std::ostream& out;
};

#endif
#endif