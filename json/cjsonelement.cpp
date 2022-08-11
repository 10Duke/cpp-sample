#include "cjsonelement.h"

#include "cjson/cJSON.h"
#include <new>
#include <memory>

namespace xdcjson = tenduke::json::cjson;

xdcjson::CjsonElement::~CjsonElement()
{
    if (root && element != nullptr) {
        cJSON_Delete(element);
    }
}

std::string xdcjson::CjsonElement::asString() const
{
    std::unique_ptr<char, decltype(std::free) *> stringRepresentation(
        cJSON_Print(element),
        std::free
    );

    if (stringRepresentation == nullptr) {
        throw std::bad_alloc();
    }

    return std::string(stringRepresentation.get());
}
