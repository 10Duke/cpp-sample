#ifndef QTJSONELEMENT_H
#define QTJSONELEMENT_H

#include "json/jsonelement.h"

namespace tenduke { namespace qt { namespace json {

class QtJSONElement: virtual public tenduke::json::JSONElement
{
public:
    QtJSONElement(enum tenduke::json::JSONElement::Type type)
        : type(type)
    {}

    virtual ~QtJSONElement() override {}

    // net::json::JSONElement implementation
public:
    virtual std::string asString() const override;
    virtual enum Type getType() const override {return type;}

private:
    enum tenduke::json::JSONElement::Type type;
};

}}}

#endif // QTJSONELEMENT_H
