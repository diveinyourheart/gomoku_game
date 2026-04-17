#ifdef MY_DEBUG

#ifndef __GOMOKUGAMEQTDEBUGLOG_H__
#define __GOMOKUGAMEQTDEBUGLOG_H__
#ifdef QT_CORE_LIB
#include <QDebug>
#include "log.h"
#include <QTextCodec>

class QtSink : public LogSink{
public:
    QtSink(){
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    }
    void write(const char* level, const std::string& message) override{
        qDebug().noquote() << QString::fromUtf8(Logger::timeNow().c_str())
            << QString::fromUtf8(Logger::delimiter.c_str())
            << QString::fromUtf8(level)
            << QString::fromUtf8(Logger::delimiter.c_str())
            << QString::fromUtf8(message.c_str());
    }
};

#endif
#endif

#endif
