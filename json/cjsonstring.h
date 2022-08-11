#ifndef CJSONSTRING_H
#define CJSONSTRING_H

#include "./cjsonelement.h"
#include "./jsonstring.h"

namespace tenduke { namespace json { namespace cjson {

class CjsonString: public tenduke::json::cjson::CjsonElement, public tenduke::json::JSONString
{
public:
    CjsonString(std::string value, cJSON * element)
        : CjsonElement(JSONElement::Type::STRING, element), value(value)
    {}
    virtual ~CjsonString() {}

    virtual std::string asString() const override
    {
        return value;
    }
    virtual const std::string & getValue() const override {return value;}

private:
    std::string value;
};

}}}

#endif // CJSONSTRING_H
