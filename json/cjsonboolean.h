#ifndef CJSONBOOLEAN_H
#define CJSONBOOLEAN_H

#include "./cjsonelement.h"
#include "./jsonboolean.h"

namespace tenduke { namespace json { namespace cjson {

const std::string TRUE = "true";
const std::string FALSE = "false";

class CjsonBoolean : public tenduke::json::cjson::CjsonElement, public tenduke::json::JSONBoolean
{
public:
    CjsonBoolean(bool value, cJSON * element)
        : CjsonElement(JSONElement::Type::BOOLEAN, element), value(value)
    {}
    virtual ~CjsonBoolean() {}

    virtual std::string asString() const override
    {
        return (value ? TRUE : FALSE);
    }
    virtual bool getValue() const override {return value;}

private:
    bool value;
};

}}}

#endif // CJSONBOOLEAN_H
