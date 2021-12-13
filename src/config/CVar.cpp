#include "CVar.h"

CVar::CVar(std::string name, double value)
{
    _name = name;
    _value = value;
    _description = "We are under construction here! Pardon the mess!";
}

CVar::CVar(std::string name, double value, bool archive)
{
    _name = name;
    _value = value;
    _archive = archive;
    _description = "We are under construction here! Pardon the mess!";
}

CVar::CVar(std::string name, double value, bool archive, std::string description)
{
    _name = name;
    _value = value;
    _archive = archive;
    _description = description;
}

CVar::~CVar() {}

double CVar::getValue()
{
    _modified = false;
    return _value;
}

std::string CVar::getDescription()
{
    return _description + "\n Current Value: " + std::to_string(_value);
}

bool CVar::isArchivable()
{
    return _archive;
}

bool CVar::wasModified()
{
    return _modified;
}

void CVar::setValue(double value)
{
    _value = value;
    _modified = true;
}

void CVar::setArchivable(bool isArchivable)
{
    _archive = isArchivable;
}

void CVar::setDescription(std::string description)
{
    _description = description;
}