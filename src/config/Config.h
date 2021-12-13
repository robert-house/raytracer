#pragma once
#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <memory>

#include "../logging/Log.h"
#include "CVar.h"
#include "CVarDescriptor.h"

// TODO: When a config is loaded from a config file, hold off on initialization until
//       the class that actually needs the variable is initialized. If a CVar is created
//       from the configuration file, it will not have the description
// TODO: Allow specific types (int, float, string, bool, etc) in CVAR
// TODO: Intelligent Type Parser (bool, string, int, float, etc.)

// Changed to singleton pattern
class Config
{
public:
    Config(Config const&) = delete;
    void operator=(Config const&) = delete;
    ~Config();

    void loadConfigFile(std::string filename);
    void print();
    void attachLogger(std::shared_ptr<Log> logger);
    static Config& getInstance() {static Config instance; return instance;}

    // Work in progress
    CVar* registerCVar(std::string name, int value, bool archive, std::string description);
    CVar* registerCVar(CVarDescriptor& descriptor);
    CVar* getCVar(std::string name);
    void parseInput(std::string lineFromConsole);

private:
    Config() {}
    std::unordered_map<std::string, CVar> _configValues;
    std::shared_ptr<Log> _pLog;

    bool parseFile(std::ifstream& file);
    std::string getNextToken(std::ifstream& file);
    bool isWhiteSpace(char c);
    bool comment(char c);
    bool isTerminalChar(char c);
    std::string readValue(std::ifstream& file);

    // TODO: Change this to public. I HATE it when games lose settings if they crash! This
    // is probably why they do!
    void saveConfig(std::string filename);
    static const int MAX_CONFIG_SIZE = 32768;
};
