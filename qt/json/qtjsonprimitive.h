#ifndef QTJSONPRIMITIVE_H
#define QTJSONPRIMITIVE_H

#include "qt/json/qtjsonelement.h"

#include <QJsonValue>

namespace tenduke { namespace qt { namespace json {

class QtJSONPrimitive : public tenduke::qt::json::QtJSONElement
{
public:
    QtJSONPrimitive(
                enum tenduke::json::JSONElement::Type type,
                QJsonValue &element
    ) : tenduke::qt::json::QtJSONElement(type), element(element)
    {}

    virtual ~QtJSONPrimitive() {}

    // tenduke::json::JSONElement implementation
    virtual std::string asString() const override;

protected:
    QJsonValue element;
};

}}}

#endif // QTJSONPRIMITIVE_H
