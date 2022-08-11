#ifndef JSONUTILS_H
#define JSONUTILS_H

#include "./jsonelement.h"
#include "./jsonobject.h"
#include <memory>
#include <string>

namespace tenduke { namespace json {

bool isBooleanTrue(const JSONElement *element);
bool isBooleanTrue(const std::unique_ptr<const JSONElement> &element);
bool isBooleanTrue(const std::shared_ptr<const JSONElement> &element);

std::string getObjectPropertyAsString(
        const json::JSONObject *object,
        const std::string &propertyName
);
std::string getObjectPropertyAsString(
        const std::unique_ptr<const json::JSONObject> &object,
        const std::string &propertyName
);

}}

#endif // JSONUTILS_H
