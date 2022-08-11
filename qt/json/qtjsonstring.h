#ifndef QTJSONSTRING_H
#define QTJSONSTRING_H

#include "json/jsonstring.h"
#include "qt/json/qtjsonprimitive.h"

#include <QJsonValue>

namespace tenduke { namespace qt { namespace json {

class QtJSONString: public tenduke::qt::json::QtJSONPrimitive, public tenduke::json::JSONString
{
public:
    QtJSONString(
                QJsonValue &element,
                std::string value
    ) : tenduke::qt::json::QtJSONPrimitive(tenduke::json::JSONElement::STRING, element), value(value)
    {}

    virtual const std::string & getValue() const override {return value;}

private:
    std::string value;
};

}}}

#endif // QTJSONSTRING_H
