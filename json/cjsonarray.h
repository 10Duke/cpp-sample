#ifndef CJSONARRAY_H
#define CJSONARRAY_H

#include "./cjsonelement.h"
#include "./jsonarray.h"

#include <memory>
#include <vector>

namespace tenduke { namespace json { namespace cjson {

class CjsonArray : public tenduke::json::cjson::CjsonElement, public tenduke::json::JSONArray
{
public:
    CjsonArray(std::unique_ptr<std::vector<std::shared_ptr<tenduke::json::JSONElement>>> values, cJSON * element)
        : CjsonElement(JSONElement::Type::ARRAY, element), values(std::move(values))
    {}

    virtual ~CjsonArray();

    // Implementations
    virtual bool isEmpty() const override {return values->empty();}
    virtual size_t getSize() const override {return values->size();}
    virtual const std::vector<std::shared_ptr<tenduke::json::JSONElement>> & getValues() override;

private:
    std::unique_ptr<std::vector<std::shared_ptr<tenduke::json::JSONElement>>> values;
};

}}}

#endif // CJSONARRAY_H
