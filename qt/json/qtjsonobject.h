#ifndef QTJSONOBJECT_H
#define QTJSONOBJECT_H

#include "json/jsonobject.h"
#include "./qtjsonelement.h"

#include <QJsonObject>

namespace tenduke { namespace qt { namespace json {

class QtJSONObject : public tenduke::qt::json::QtJSONElement, public tenduke::json::JSONObject
{
public:
    QtJSONObject(
        QJsonObject &element,
        std::map<std::string, std::shared_ptr<tenduke::json::JSONElement>> properties
    ) : tenduke::qt::json::QtJSONElement(tenduke::json::JSONElement::OBJECT),
        element(element), properties(properties)
    {}

    virtual ~QtJSONObject() {}

public:
    // tenduke::json::JSONElement implementation
    virtual std::string asString() const override;

    // tenduke::json::JSONObject implementation
    virtual std::shared_ptr<JSONElement> getProperty(const std::string & propertyName) const override;
    virtual bool hasProperty(const std::string & propertyName) const override;
    virtual std::shared_ptr<JSONElement> removeProperty(const std::string & propertyName) override;

    virtual const std::map<std::string, std::shared_ptr<JSONElement>> & getProperties() const override;

private:
    QJsonObject element;
    std::map<std::string, std::shared_ptr<tenduke::json::JSONElement>> properties;
};

}}}

#endif // QTJSONOBJECT_H
