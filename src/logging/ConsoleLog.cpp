#include "ConsoleLog.h"

// TODO: Fix appendNextLine being called and giving an error message
// TODO: Setting verbose mode to false will only print section and sub-section heads
// TODO: Add ability to nest sections
// TODO: Add ability to set a log level for output

ConsoleLog::ConsoleLog(std::string logID) : Log(logID)
{
    _logLevel = INFO;
    _sectionOpen = false;
    _verbose = false;
    _errorTriggered = false;
}

ConsoleLog::ConsoleLog(LogLevel level, bool verbose) : Log("console")
{
    _logLevel = level;
    _sectionOpen = false;
    _verbose = verbose;
    _errorTriggered = false;
}

ConsoleLog::~ConsoleLog()
{
    if (_sectionOpen)
    {
        std::cout << "----SECTION WAS NOT CLOSED----" << std::endl;
        closeSection("?");
        std::cout << "------YOU DIDN'T CLOSE A SECTION!------" << std::endl;
    }
}

void ConsoleLog::log(LogLevel level, std::string message)
{
    if(_sectionOpen)
    {
        _buffer.push_back(logout(level, message));
    }
    else
    {
        // FIXME: Not working for some reason
        if (level <= _logLevel)
        {
            std::cout << logout(level, message) << std::endl;
        }
    }
}

void ConsoleLog::info(std::string message)
{
    log(INFO, message);
}

void ConsoleLog::warn(std::string message)
{
    log(WARNING, message);
}

void ConsoleLog::debug(std::string message)
{
    log(DEBUG, message);
}

void ConsoleLog::error(std::string message)
{
    log(ERR, message);
    _errorTriggered = true;
}

void ConsoleLog::crit(std::string message)
{
    log(CRIT, message);
}

void ConsoleLog::openSection(std::string sectionPrefix)
{
    if (_sectionOpen)
    {
        std::cout << "----SECTION WAS NOT CLOSED----" << std::endl;
        closeSection("?");
        std::cout << "------YOU DIDN'T CLOSE A SECTION!------" << std::endl;
    }

    std::cout << logout(INFO, sectionPrefix);
    _sectionOpen = true;
    _errorTriggered = false;
}

void ConsoleLog::closeSection(std::string sectionPostfix)
{
    std::cout << sectionPostfix << std::endl;

    // Only print details if verbosity is enabled
    if (_verbose || _errorTriggered)
    {
        // TODO: Limit output based on log level here
        // Dump buffer
        for (std::string bufferedLog : _buffer)
        {
            std::cout << "\t|->" << bufferedLog << std::endl;
        }

        if (!_buffer.empty())
        {
            std::cout << "\t------------------------------------------------------" << std::endl;
        }
    }

    // Clear buffer
    _buffer.clear();

    // Close Section
    _sectionOpen = false;
    _errorTriggered = false;
}

void ConsoleLog::setVerbosity(bool enableVerbosity)
{
    _verbose = enableVerbosity;
}

// Private Members
std::string ConsoleLog::logout(LogLevel level, std::string message)
{
    std::string output = "";

    switch (level)
    {
    case DEBUG:
        output.append("[DEBUG] ");
        break;
    case INFO:
        output.append("[INFO] ");
        break;
    case WARNING:
        output.append("[WARN] ");
        break;
    case ERR:
        output.append("[ERROR] ");
        break;
    }

    output.append(message);

    return output;
}
