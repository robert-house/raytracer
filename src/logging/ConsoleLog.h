#pragma once
#include "Log.h"
#include <iostream>
#include <vector>

class ConsoleLog final : public Log
{
public:
    ConsoleLog();
    ConsoleLog(LogLevel logLevel, bool verbose);
    ~ConsoleLog();

    void log(LogLevel level, std::string message) override;

    void info(std::string message) override;
    void warn(std::string message) override;
    void debug(std::string message) override;
    void error(std::string message) override;

    void openSection(std::string sectionPrefix) override;
    void closeSection(std::string sectionPostfix) override;

    void setVerbosity(bool enableVerbose) override;

protected:
    std::string logout(LogLevel level, std::string message); // TODO: Change the name. Its bad
    std::vector<std::string> _buffer;
    bool _errorTriggered;
};
