/**
 * @author Jonas Ransjö
 */

#ifndef CVARS_H
#define CVARS_H

#include <iostream>
#include <string>

#include "rtypes.h"

namespace rc {

class CVar
{
public:

    enum Flags {
        CVAR_ALL = ~0,
        CVAR_READONLY = 1 << 0,
        CVAR_MODIFIED = 1 << 2,
        CVAR_DEFAULT_FLAGS = 0
    };

    enum Type {
        CVAR_STRING = 0,
        CVAR_BOOL = 1,
        CVAR_INT = 2,
        CVAR_FLOAT =  3
    };


    CVar(Type type, const std::string& name, const std::string& value,
         const std::string& description = "No info", uint flags = CVAR_DEFAULT_FLAGS);

    CVar(const std::string& name, const std::string& value,
         const std::string& description = "No info", uint flags = CVAR_DEFAULT_FLAGS)
        : CVar(CVAR_STRING, name, value, description, flags) {}

    CVar(const std::string& name, bool value,
         const std::string& description = "No info", uint flags = CVAR_DEFAULT_FLAGS)
        : CVar(CVAR_BOOL, name, std::to_string(value), description, flags) {}

    CVar(const std::string& name, int value,
         const std::string& description = "No info", uint flags = CVAR_DEFAULT_FLAGS)
        : CVar(CVAR_INT, name, std::to_string(value), description, flags) {}

    CVar(const std::string& name, float value,
         const std::string& description = "No info", uint flags = CVAR_DEFAULT_FLAGS)
        : CVar(CVAR_FLOAT, name, std::to_string(value), description, flags) {}

    ~CVar() {}

    std::string getName() { return name; }
    Type getType() { return type; }
    uint getFlags() { return flags; }
    std::string getDescription() { return description; }
    std::string getString() { return value; }

    bool isModified() { return (flags & CVAR_MODIFIED) != 0; }
    void setModified() { flags |= CVAR_MODIFIED; }
    void clearModified() { flags &= ~CVAR_MODIFIED; }

    bool getBool() { return intValue == 0 ? false : true; }
    int getInt() { return intValue; }
    float getFloat() { return floatValue; }

    void setString(const std::string& value);
    void setBool(bool value) { setString(std::to_string(value)); }
    void setInt(int value) { setString(std::to_string(value)); }
    void setFloat(float value) { setString(std::to_string(value)); }

    void set(const std::string& value) { setString(value); }

    template <typename T>
    void set(const T& value) { setString(std::to_string(value)); }

    void reset() { value = defaultValue; }

private:
    Type type;
    uint flags;
    std::string name;
    std::string description;
    std::string value;
    std::string defaultValue;
    std::string oldValue;
    int intValue;
    float floatValue;

    void updateValues();
};

inline CVar::CVar(Type type, const std::string& name, const std::string& value,
                  const std::string& description, uint flags)
{
    this->type = type;
    this->name = name;
    this->value = value;
    this->description = description;
    this->flags = flags;
}

inline void CVar::setString(const std::string& value)
{
    if (flags & CVAR_READONLY) {
        std::cout << "CVar " << name << " is readonly." << std::endl;
        return;
    }

    if (value.compare(this->value) == 0)
        return;

    oldValue = this->value;
    this->value = value;

    updateValues();
    setModified();
}

inline void CVar::updateValues()
{
    switch (type) {
    case CVAR_BOOL:
        intValue = (std::stoi(value) != 0);
        floatValue = intValue;
        value = intValue ? "1" : "0";
        break;
    case CVAR_INT:
        intValue = std::stoi(value);
        floatValue = intValue;
        value = std::to_string(intValue);
        break;
    case CVAR_FLOAT:
        floatValue = std::stof(value);
        intValue = (int) floatValue;
        value = std::to_string(floatValue);
        break;
    default:
        floatValue = 0;
        intValue = 0.f;
    }
}

inline std::ostream &operator<<(std::ostream &out, const CVar& cvar)
{
    out << cvar.getString();
    return out;
}

} // namespace
#endif

