/**
 * @author Jonas Ransjö
 */

#ifndef CVARS_H
#define CVARS_H

#include <string>
#include <sstream>
#include <iostream>
#include <boost/variant.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <glm/glm.hpp>
#include "math/glmutils.h"

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
        CVAR_FLOAT =  3,
        CVAR_DOUBLE = 4,
        CVAR_VEC2 = 5,
        CVAR_VEC3 = 6,
        CVAR_VEC4 = 7
    };

    typedef boost::variant<
        std::string,
        bool,
        int,
        float,
        double,
        glm::vec2,
        glm::vec3,
        glm::vec4
        > ValueType;

    struct to_string_visitor : boost::static_visitor<std::string>
    {
        std::string operator()(const std::string& val) {
            return val; }
        std::string operator()(const bool& val) {
            return val ? "true" : "false"; }
        std::string operator()(const int& val) {
            return std::to_string(val); }
        std::string operator()(const float& val) {
            return std::to_string(val); }
        std::string operator()(const double& val) {
            return std::to_string(val); }
        template <typename T>
        std::string operator()(const T& val)
        {
            std::ostringstream os;
            os << val;
            return os.str();
        }
    } to_string_visitor;

    struct from_string_visitor : boost::static_visitor<>
    {
        from_string_visitor(const std::string& str) {
            this->str = str;
        }
        std::string str;

        void operator()(std::string& val) const {
            val = str;
        }
        void operator()(bool& val) const {
            val = str == "0" || str == "false" || str == "FALSE";
        }
        void operator()(int& val) const {
            val = std::stoi(str);
        }
        void operator()(float& val) const {
            val = std::stof(str);
        }
        void operator()(double& val) const {
            val = std::stod(str);
        }
        template <typename T>
        void operator()(T& val) const
        {
            std::string vstr(str);
            for (char& c : vstr)
                if (c == '(' || c == ',' || c == ')')
                    c = ' ';
            std::istringstream is(vstr);
            is >> val;
        }
    };

    template<typename T>
    CVar(const std::string& name, const T& value,
         const std::string& description = "No info",
         uint flags = CVAR_DEFAULT_FLAGS);

    ~CVar() {}

    std::string getName() const { return name; }
    uint getType() const { return type; }
    uint getFlags() const { return flags; }
    std::string getDescription() const { return description; }
    std::string getString() const { return stringValue; }

    bool isModified() const { return (flags & CVAR_MODIFIED) != 0; }
    void setModified() { flags |= CVAR_MODIFIED; }
    void clearModified() { flags &= ~CVAR_MODIFIED; }

    template<typename T>
    T get() const { return boost::get<T>(value); }

    template<typename T>
    void set(const T& value);

    void setValue(const ValueType& value);
    void setString(const std::string& value);

    void reset() { value = defaultValue; }

private:
    uint type;
    uint flags;
    std::string name;
    std::string description;
    std::string stringValue;
    ValueType value;
    ValueType defaultValue;
    ValueType oldValue;

    void updateString();
    void updateValue();
};

template<typename T>
inline CVar::CVar(const std::string& name, const T& value,
                  const std::string& description,
                  uint flags)
{
    this->name = name;
    this->value = value;
    this->description = description;
    this->flags = flags;

    this->defaultValue = value;
    this->oldValue = value;
    this->type = this->value.which();
    updateString();
}

template<typename T>
inline void CVar::set(const T& value)
{
    ValueType val = value;
    setValue(val);
}

inline void CVar::setValue(const ValueType& value)
{
    if (flags & CVAR_READONLY) {
        std::cerr << "CVar " << name << " is readonly." << std::endl;
        return;
    }
    if (value.which() != type) {
        std::cerr << "Incorrect type." << std::endl;
        return;
    }

    if (value == this->value)
        return;

    this->value = value;
    updateString();
    setModified();
}

inline void CVar::setString(const std::string& value)
{
    if (flags & CVAR_READONLY) {
        std::cerr << "CVar " << name << " is readonly." << std::endl;
        return;
    }

    if (value.compare(stringValue) == 0)
        return;

    stringValue = value;
    updateValue();
    updateString();
    setModified();
}

inline void CVar::updateString()
{
    stringValue = boost::apply_visitor(to_string_visitor, value);
}

inline void CVar::updateValue()
{
    oldValue = value;
    boost::apply_visitor(from_string_visitor(stringValue), value);
}

inline std::ostream &operator<<(std::ostream &out, const CVar& cvar)
{
    out << cvar.getString();
    return out;
}

} // namespace

#endif

