#pragma once
#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

class Log
{
public:
    enum LogLevel
    {
        ERR = 0,
        WARNING,
        INFO,
        DEBUG,
        CRIT
    };

    Log(std::string logID) { _logLevel = INFO; logger = spdlog::get(logID);};
    virtual ~Log() {};

    virtual void log(LogLevel level, std::string message) = 0;

    virtual void info(std::string message) = 0;
    virtual void warn(std::string message) = 0;
    virtual void debug(std::string message) = 0;
    virtual void crit(std::string message) = 0;
    virtual void error(std::string message) = 0;

    virtual void openSection(std::string sectionPrefix) = 0;
    virtual void closeSection(std::string sectionPostfix) = 0;
    virtual void setVerbosity(bool enableVerbose) = 0;

protected:
    LogLevel _logLevel;
    bool _sectionOpen;
    bool _verbose;
    std::shared_ptr<spdlog::logger> logger;
};