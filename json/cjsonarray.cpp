#include "./cjsonarray.h"

#include "cjson/cJSON.h"

namespace nsapi = tenduke::json;
namespace ns = tenduke::json::cjson;

ns::CjsonArray::~CjsonArray()
{
    if (values != nullptr) {
        for (auto value: *(values.get())) {

        }
    }
}

const std::vector<std::shared_ptr<nsapi::JSONElement>> & ns::CjsonArray::getValues()
const
{
    return *(values.get());
}
