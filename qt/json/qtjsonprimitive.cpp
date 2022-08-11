#include "./qtjsonprimitive.h"

namespace qtjson = tenduke::qt::json;
namespace xdjson = tenduke::json;

std::string qtjson::QtJSONPrimitive::asString() const
{
    if (element.isBool()) {
        return (element.toBool() ? "true" : "false");
    }
    else if (element.isDouble()) {
        return std::to_string(element.toDouble());
    }
    else if (element.isNull()) {
        return "null";
    }
    else if (element.isString()) {
        return element.toString().toStdString();
    }
    else {
        return "undefined";
    }
}

