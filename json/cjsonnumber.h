#ifndef CJSONNUMBER_H
#define CJSONNUMBER_H

#include "./cjsonelement.h"
#include "./jsonnumber.h"

namespace tenduke { namespace json { namespace cjson {

class CjsonNumber : public tenduke::json::cjson::CjsonElement, public tenduke::json::JSONNumber
{
public:
    CjsonNumber(double value, cJSON * element)
        : CjsonElement(JSONElement::Type::NUMBER, element), value(value)
    {}
    virtual ~CjsonNumber() {}

    virtual std::string asString() const override
    {
        return std::to_string(value);
    }
    virtual double getValue() const override {return value;}

private:
    double value;
};

}}}

#endif // CJSONNUMBER_H
