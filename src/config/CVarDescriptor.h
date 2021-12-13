#pragma once

#include <string>

class CVarDescriptor
{
public:
    CVarDescriptor() {}
    ~CVarDescriptor() {}

    // Fields
    std::string name = "";
    std::string description = "";
    float value = 0.0;
    float defaultValue = 0.0;
    bool archivable = false;

};