#pragma once
#include <memory>
#include "logging/Log.h"
#include "logging/ConsoleLog.h"
#include "config/Config.h"

class Component
{
public:
    // If no log is given, create a new console log to pass in
    Component() { _pLog = std::shared_ptr<Log>(new ConsoleLog("console")); }
    Component(std::shared_ptr<Log> log) { _pLog = log; }
    ~Component();

protected:
    std::shared_ptr<Log> _pLog;
    Config& _config = Config::getInstance();
};