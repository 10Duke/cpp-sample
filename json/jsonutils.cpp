#include "jsonutils.h"

#include "./jsonboolean.h"

namespace json = tenduke::json;

bool json::isBooleanTrue(const json::JSONElement * element)
{
  if (element != nullptr && element->isBoolean()) {
      return dynamic_cast<const json::JSONBoolean *>(element)->getValue();
  }
  return false;
}

bool json::isBooleanTrue(const std::unique_ptr<const json::JSONElement> &element)
{
    return json::isBooleanTrue(element.get());
}

bool json::isBooleanTrue(const std::shared_ptr<const json::JSONElement> &element)
{
    return json::isBooleanTrue(element.get());
}

std::string json::getObjectPropertyAsString(
        const json::JSONObject *object,
        const std::string &propertyName
)
{
    if (object != nullptr) {
        const std::shared_ptr<json::JSONElement> property = object->getProperty(propertyName);
        if (property != nullptr) {
            return property->asString();
        }
    }
    return "";
}

std::string json::getObjectPropertyAsString(
        const std::unique_ptr<const json::JSONObject> &object,
        const std::string &propertyName
)
{
    return json::getObjectPropertyAsString(object.get(), propertyName);
}
