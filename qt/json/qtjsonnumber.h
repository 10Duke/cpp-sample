#ifndef QTJSONNUMBER_H
#define QTJSONNUMBER_H

#include "json/jsonnumber.h"
#include "qt/json/qtjsonprimitive.h"

#include <QJsonValue>

namespace tenduke { namespace qt { namespace json {

class QtJSONNumber : public tenduke::qt::json::QtJSONPrimitive, public tenduke::json::JSONNumber
{
public:
    QtJSONNumber(
                QJsonValue &element,
                double value
    ) : tenduke::qt::json::QtJSONPrimitive(tenduke::json::JSONElement::NUMBER, element), value(value)
    {}

    virtual double getValue() const override {return value;}

private:
    double value;
};

}}}

#endif // QTJSONNUMBER_H
