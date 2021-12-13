#pragma once
#include <string>

class CVar
{
public:
    CVar() {}
    CVar(std::string name, double value);
    CVar(std::string name, double value, bool archive);
    CVar(std::string name, double value, bool archive, std::string description);
    ~CVar();

    double getValue();
    std::string getDescription();
    bool isArchivable();
    bool wasModified();
    void setValue(double value);
    void setArchivable(bool isArchivable);
    void setDescription(std::string description);

private:
    std::string _name;
    std::string _description;
    double _value;
    bool _archive;
    bool _modified; // Modified since last use
};