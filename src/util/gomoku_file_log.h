#ifdef MY_DEBUG

#ifndef __GOMOKUGAMEFILELOG_H__
#define __GOMOKUGAMEFILELOG_H__
#include "log.h"
#include <fstream>

class FileLogSink : public LogSink{
public:
    explicit FileLogSink(const std::string& filename){
        file.open(filename, std::ios::app);
    }
    void write(const char* level, const std::string& message) override{
        file << Logger::timeNow()
            << Logger::delimiter
            << level
            << Logger::delimiter
            << message;
    }
private:
    std::ofstream file;
};
#endif

#endif