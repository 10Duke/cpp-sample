#include "./cjsonobject.h"

#include "cjson/cJSON.h"

namespace cjson = tenduke::json::cjson;
namespace json = tenduke::json;

cjson::CjsonObject::~CjsonObject()
{
}

std::shared_ptr<json::JSONElement> cjson::CjsonObject::getProperty(const std::string &propertyName) const
{
    auto property = properties->find(propertyName);

    if (property == properties->end()) {
        return nullptr;
    }

    return property->second;
}

bool cjson::CjsonObject::hasProperty(const std::string &propertyName) const
{
    auto property = properties->find(propertyName);

    return (property != properties->end());
}

const std::map<std::string, std::shared_ptr<json::JSONElement>> & cjson::CjsonObject::getProperties() const
{
    return *(properties.get());
}

std::shared_ptr<json::JSONElement> cjson::CjsonObject::removeProperty(const std::string &propertyName)
{
    std::shared_ptr<json::JSONElement> property = getProperty(propertyName);

    // There is such property:
    if (property != nullptr) {
        properties->erase(propertyName);

        // Detach the property from the cJSON-object.
        // The item returned by the call should the same within the "property".
        cJSON * item = cJSON_DetachItemFromObjectCaseSensitive(element, propertyName.c_str());

        // Make the element "root": It is now a self-contained and should be de-allocated when
        // out-of-scope:
        cjson::CjsonElement * detached = dynamic_cast<cjson::CjsonElement *>(property.get());
        detached->setRoot(true);
    }

    return property;
}
