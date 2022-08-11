#include "qtjsonobject.h"

namespace qtjson = tenduke::qt::json;
namespace xdjson = tenduke::json;

std::shared_ptr<xdjson::JSONElement> qtjson::QtJSONObject::getProperty(const std::string &propertyName) const
{
    auto property = properties.find(propertyName);

    if (property == properties.end()) {
        return nullptr;
    }

    return property->second;
}

bool qtjson::QtJSONObject::hasProperty(const std::string &propertyName) const
{
    auto property = properties.find(propertyName);

    return (property != properties.end());
}

const std::map<std::string, std::shared_ptr<xdjson::JSONElement>> & qtjson::QtJSONObject::getProperties() const
{
    return properties;
}

std::shared_ptr<xdjson::JSONElement> qtjson::QtJSONObject::removeProperty(const std::string &propertyName)
{
    std::shared_ptr<xdjson::JSONElement> property = getProperty(propertyName);

    // There is such property:
    if (property != nullptr) {
        element.remove(QString::fromStdString(propertyName));
    }

    return property;
}

std::string qtjson::QtJSONObject::asString()
const
{
    std::string result = "{";

    bool first = true;
    for (auto const &property : properties) {
        if (first) {
            first = false;
        }
        else {
            result += ',';
        }
        result += '\"';
        result += property.first;
        result += "\":";
        result += property.second->asString();
    }

    result += '}';

    return result;
}
