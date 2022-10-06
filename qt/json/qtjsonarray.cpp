#include "./qtjsonarray.h"

namespace qtjson = tenduke::qt::json;
namespace xdjson = tenduke::json;

bool qtjson::QtJSONArray::isEmpty()
const
{
    return arrayElement.isEmpty();
}

size_t qtjson::QtJSONArray::getSize()
const
{
    return arrayElement.size();
}

const std::vector<std::shared_ptr<xdjson::JSONElement>> & qtjson::QtJSONArray::getValues()
const
{
    return items;
}

std::string qtjson::QtJSONArray::asString()
const
{
    std::string result = "[";
    int numItems = items.size();

    if (numItems > 0) {
        result + items[0]->asString();

        for (int i = 1; i < numItems; i++) {
            result += ',';
            result + items[i]->asString();
        }
    }

    result += ']';

    return result;
}
