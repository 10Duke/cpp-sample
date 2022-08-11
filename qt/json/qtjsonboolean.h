#ifndef QTJSONBOOLEAN_H
#define QTJSONBOOLEAN_H

#include "json/jsonboolean.h"
#include "qt/json/qtjsonprimitive.h"

#include <QJsonValue>

namespace tenduke { namespace qt { namespace json {

class QtJSONBoolean : public tenduke::qt::json::QtJSONPrimitive, public tenduke::json::JSONBoolean
{
public:
    QtJSONBoolean(
                QJsonValue &element,
                bool value
    ) : tenduke::qt::json::QtJSONPrimitive(tenduke::json::JSONElement::BOOLEAN, element), value(value)
    {}

    virtual bool getValue() const override {return value;}

private:
    bool value;
};

}}}

#endif // QTJSONBOOLEAN_H
