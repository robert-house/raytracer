#include "Config.h"

// TODO: Add ability to dump ALL config values to disk

Config::~Config()
{
    using namespace std;
    /*
    for (std::pair<std::string, CVar> element : _configValues)
    {
	    std::cout << element.first << " :: " << element.second.getValue()
        << " :: " << element.second.isArchivable() << std::endl;
    }
    */

   // SEGFAULT fix
   if (!_configValues.empty())
   {
       saveConfig("user.cfg"); // TODO: Change the hardcoding

        // Cleanup CVARS
        _configValues.clear();
   }
}

void Config::loadConfigFile(std::string filename)
{
    // Open file
    std::ifstream file(filename);
    _pLog->openSection("Loading file [" + filename + "]. Please Stand By...");

    // Grab the first line
    std::string line;
    std::string token = "";
    std::deque<std::string> tokens;

    // Parse through the line
    if (file.is_open())
    {
        while (!file.eof())
        {
            if(!parseFile(file))
            {
                _pLog->closeSection("FAIL");
                return;
            }
        }

        // Doing this for the sake of doing this
        if (!token.empty())
        {
            std::cout << token << std::endl;
        }

        // Close file when done
        file.close();
    }
    else
    {
        // File does not exist
        // TODO: Log warning that log did not open properly
        // TODO: Create a default configuration file if not found
        _pLog->closeSection("FAIL");
        _pLog->error("FILE NOT FOUND");
    }

    _pLog->closeSection("DONE");

    // Build auto complete as input is being read in
    // TODO: Think of a solid system to get all of the possible configurable
    //       elements in the code;
    // Idea: When the components call to grab values and they do not exitst,
    //       register them in the map and return null
}

void Config::print()
{
    _pLog->log(Log::INFO, "============ CONFIG CONTENTS ============");

    for (std::pair<std::string, CVar> element : _configValues)
    {
	    _pLog->log(Log::INFO,
                        "\t"
                        + element.first + " :: "
                        + std::to_string(element.second.getValue()) + " :: "
                        + std::to_string(element.second.isArchivable()));

        //std::cout << element.first << " :: " << element.second.getValue()
        //<< " :: " << element.second.isArchivable() << std::endl;
    }

    _pLog->log(Log::INFO, "=========================================");
}

void Config::attachLogger(std::shared_ptr<Log> logger)
{
    _pLog = logger;
}

CVar* Config::registerCVar(std::string name, int value, bool archive, std::string description)
{
    CVar* temp = getCVar(name);

    if (temp != nullptr)
    {
        // Exists
        _pLog->warn("CVar already exists with the name "
                        + name
                        + "! "
                        + "Returning Existing Value of: "
                        + std::to_string(temp->getValue()));

        // Do update whether we want to write this to the config though
        temp->setArchivable(archive);
        temp->setDescription(description);

        return temp;
    }
    else
    {
        _configValues[name] = CVar(name, value, archive, description);
        return &_configValues[name];
    }
}

CVar* Config::registerCVar(CVarDescriptor& descriptor)
{
    return registerCVar(descriptor.name,
                        descriptor.value,
                        descriptor.archivable,
                        descriptor.description);
}

CVar* Config::getCVar(std::string name)
{
    // Getting iterator from the map
    auto temp = _configValues.find(name);

    if (temp != _configValues.end())
    {
        return &temp->second;
    }

    // Not found!
    return nullptr;
}

void Config::parseInput(std::string inputToParse)
{
    // Input will always be valid in the form of:
    // [command/var] [value] or [command/var]

    // Just do a hack here and split by whitespace. Not secure, but will do for now!
    std::string delimiter = " ";
    std::vector<std::string> tokens;
    std::string token = inputToParse.substr(0, inputToParse.find(delimiter));
    bool endOfInput = false;

    int i = 0;

    // Tokenize the line of input
    while (!endOfInput)
    {
        if (inputToParse.find(delimiter) == std::string::npos)
        {
            endOfInput = true;

            if (inputToParse != "")
            {
                tokens.push_back(inputToParse);
            }
        }
        else
        {
            inputToParse.erase(0, inputToParse.find(delimiter) + delimiter.length());

            if (token != "")
            {
                tokens.push_back(token);
            }

            token = inputToParse.substr(0, inputToParse.find(delimiter));
        }
    }

    CVar* cvar = getCVar(tokens[0]);

    // If the CVar was found, process it
    if (cvar != nullptr)
    {
        // Do processing
        if (tokens.size() == 1)
        {
            // Print out the usage guide
            std::cout << cvar->getDescription() << std::endl;
        }
        else
        {
            // Process command
            if (tokens.size() > 2 && tokens[1] == "=")
            {
                cvar->setValue(std::atof(tokens[2].c_str()));
            }
            else
            {
                cvar->setValue(std::atof(tokens[1].c_str()));
            }

            _pLog->info(tokens[0] + " successfuly set");
        }
    }
    else
    {
        _pLog->info("CVar " + tokens[0] + " is invalid!");
    }

}

bool Config::parseFile(std::ifstream& file)
{
    // Configuration grammar is as follows:
    //      <config>     ::= {<comment> | <command>} ';'
    //      <command>    ::= <namespace> '.' [<command> | <variable>] '=' <value> ';'
    //      <comment>    ::= '#' {<letter>}
    //      <value>      ::= <integer> | <bool> | <float> | <string_lit>
    //      <string_lit> ::= '"' {<letter>} '"'
    //      <float>      ::= {<number>} '.' {<number>}
    //      <letter>     ::= 'a' | 'b' | ... | 'z'
    //      <number>     ::= '1' | '2' | ... | '9' | '0'

    // Discard whitespace unless string literal or comment
    std::string token = getNextToken(file);
    std::string name = "";
    std::string value = "";
    CVar* pCVar = nullptr;
    unsigned int charCount = 0;

    while (token != "")
    {
        charCount += token.size();

        // Do a size check and return with an error if we exceed the max char limit
        if (charCount > MAX_CONFIG_SIZE)
        {
            _pLog->error("Configuation loader has exceeded the maximum configuration size! (" + std::to_string(charCount) + ")");
            return false;
        }

        //TODO: Re-add support for comments
        if (token == "=")
        {
            value = readValue(file);
            pCVar = getCVar(name);

            if (pCVar == nullptr)
            {
                _pLog->warn("Could not find CVar " + name);
            }
            else
            {
                // Add Value to Map
                pCVar->setValue(std::atof(value.c_str()));
                //_configValues[name] = CVar(name, std::atof(value.c_str()), false);

                pCVar = nullptr;
            }

            // Log that a cvar was read from file
            _pLog->debug(name + "=" + value + " was successfully read");

            // Clear String
            name.clear();
        }
        else
        {
            name += token;
        }

        token = getNextToken(file);
    }

    // new line or semicolon
    return true;
}

std::string Config::getNextToken(std::ifstream& file)
{
    char tempChar;
    std::string token = "";

    // TODO: Maybe return empty string if there is no chars left in the stream
    while (!file.eof())
    {
        tempChar = file.peek();

        if (isTerminalChar(tempChar))
        {
            if (token.empty())
            {
                // Terminal char only
                token += file.get();
            }

            return token;
        }
        else
        {
            // Accumulate if not whitespace
            if (!isWhiteSpace(tempChar))
            {
                token += file.get();
            }
            else
            {
                file.get(); // Just pop it and lock it!
            }
        }
    }

    // If this gets down here, file empty. Return empty string
    if (token != "")
    {
        return token;
    }

    return "";
}

// Helper Functions
bool Config::isWhiteSpace(char c)
{
    if (c == ' ' || c == '\t' || c == '\n')
    {
        return true;
    }

    return false;
}

bool Config::comment(char c)
{
    return false;
}

bool Config::isTerminalChar(char c)
{
    switch (c)
    {
        case '.':
            return true;
        case ';':
            return true;
        case '=':
            return true;
        case '#':
            return true;
        case '\xff': // Perhaps remove later
            return true;
        default:
            return false;
    };
}

std::string Config::readValue(std::ifstream& file)
{
    // Next token should be a value
    std::string value = "";
    std::string token = getNextToken(file);

    // Parse Value
    while (token != ";")
    {
        value += token;
        token = getNextToken(file);
    }

    if (token.empty())
    {
        // Something happened
        _pLog->error("Error Reading Value!");
    }

    return value;
}

void Config::saveConfig(std::string filename)
{
    std::ofstream file;

    file.open(filename.c_str());

    _pLog->openSection("Writing Config to Disk...");

    for (std::pair<std::string, CVar> element : _configValues)
    {
        if (element.second.isArchivable())
        {
            file << element.first << " = " << element.second.getValue() << ";\n";
        }
    }

    file.close();

    _pLog->closeSection("DONE");
}