#include "./cjsonparser.h"
#include "./cjsonarray.h"
#include "./cjsonboolean.h"
#include "./cjsonelement.h"
#include "./cjsonnumber.h"
#include "./cjsonobject.h"
#include "./cjsonstring.h"
#include "./jsonparsingexception.h"

#include <cjson/cJSON.h>
#include <memory>
#include <stdexcept>
#include <vector>

namespace nsapi = tenduke::json;
namespace ns = tenduke::json::cjson;


static ns::CjsonElement * buildElement(cJSON * element);


ns::cJSONParser::cJSONParser()
{
}


static ns::CjsonArray * buildArray(cJSON * arrayElement)
{
    // Build children first:
    std::unique_ptr<std::vector<std::shared_ptr<nsapi::JSONElement>>> values (new std::vector<std::shared_ptr<nsapi::JSONElement>>());
    values->reserve(cJSON_GetArraySize(arrayElement));

    cJSON * current = arrayElement->child;
    while(current != nullptr) {
        values->push_back(std::shared_ptr<nsapi::JSONElement>(buildElement(current)));
        current = current->next;
    }

    return new ns::CjsonArray(std::move(values), arrayElement);
}


static ns::CjsonElement * buildObject(cJSON * objectElement)
{
    std::unique_ptr<std::map<std::string, std::shared_ptr<nsapi::JSONElement>>> properties (new std::map<std::string, std::shared_ptr<nsapi::JSONElement>>());

    cJSON * current = objectElement->child;
    while (current != nullptr) {
        properties->emplace(
            std::string(current->string),
            std::shared_ptr<nsapi::JSONElement>(buildElement(current))
        );
        current = current->next;
    }

    return new ns::CjsonObject(std::move(properties), objectElement);
}


static ns::CjsonElement * buildElement(cJSON * element)
{
    if (cJSON_IsNull(element)) {
        return new ns::CjsonElement(nsapi::JSONElement::Type::NULLISH, element);
    }
    else if (cJSON_IsInvalid(element)) {
        return new ns::CjsonElement(nsapi::JSONElement::Type::UNDEFINED, element);
    }
    else if (cJSON_IsFalse(element)) {
        return new ns::CjsonBoolean(false, element);
    }
    else if (cJSON_IsTrue(element)) {
        return new ns::CjsonBoolean(true, element);
    }
    else if (cJSON_IsNumber(element)) {
        return new ns::CjsonNumber(element->valuedouble, element);
    }
    else if (cJSON_IsString(element)) {
        return new ns::CjsonString(std::string(element->valuestring), element);
    }
    else if (cJSON_IsArray(element)) {
        return buildArray(element);
    }
    else if (cJSON_IsObject(element)) {
        return buildObject(element);
    }
    else {
        // Also covers cJSON_IsInvalid()
        return new ns::CjsonElement(nsapi::JSONElement::Type::UNDEFINED, element);
    }
}


static ns::CjsonElement * buildTree(cJSON * element)
{
  ns::CjsonElement * root = buildElement(element);

  root->setRoot(true);

  return root;
}


std::unique_ptr<nsapi::JSONElement> ns::cJSONParser::from(
        const char * const jsonAsString,
        size_t length
) const
{
     const char * errorPtr = nullptr;
     cJSON *parsedJson = cJSON_ParseWithLengthOpts(
                 jsonAsString,
                 length,
                 &errorPtr,
                 0 // Don't require that the JSON immediately ends to \0, allow whitespace?
     );

     if (parsedJson == nullptr) {
         int offset = errorPtr - jsonAsString;

         throw nsapi::JSONParsingException("JSON parsing failed at position: " + std::to_string(offset));
     }

     return std::unique_ptr<nsapi::JSONElement>(buildTree(parsedJson));
}

std::unique_ptr<nsapi::JSONElement> ns::cJSONParser::from(const std::string &jsonAsString) const
{
    return from(jsonAsString.c_str(), jsonAsString.size());
}
