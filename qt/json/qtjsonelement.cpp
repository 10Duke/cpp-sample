#include "./qtjsonelement.h"

namespace qtjson = tenduke::qt::json;
namespace xdjson = tenduke::json;

std::string qtjson::QtJSONElement::asString() const
{
    switch(type) {
        case xdjson::JSONElement::NULLISH:
            return "null";
        default:
            return "undefined";
    }
}

