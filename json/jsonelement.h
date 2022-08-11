#ifndef JSONELEMENT_H
#define JSONELEMENT_H

#include <string>

namespace tenduke { namespace json {

class JSONElement
{
public:
    enum Type {
      UNDEFINED,
      NULLISH,
      OBJECT,
      ARRAY,
      NUMBER,
      STRING,
      BOOLEAN
    };

public:
    virtual ~JSONElement() {}

    virtual enum Type getType() const = 0;
    virtual std::string asString() const = 0;

    // Utilities to determine type of the element
    bool isArray() const {return (getType() == Type::ARRAY);}
    bool isBoolean() const {return (getType() == Type::BOOLEAN);}
    bool isNull() const {return (getType() == Type::NULLISH);}
    bool isNumber() const {return (getType() == Type::NUMBER);}
    bool isObject() const {return (getType() == Type::OBJECT);}
    bool isString() const {return (getType() == Type::STRING);}
    bool isUndefined() const {return (getType() == Type::UNDEFINED);}

};

}}

#endif // JSONELEMENT_H
