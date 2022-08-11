#ifndef CJSONOBJECT_H
#define CJSONOBJECT_H

#include "./cjsonelement.h"
#include "./jsonobject.h"

#include <memory>

namespace tenduke { namespace json { namespace cjson {

class CjsonObject : public tenduke::json::cjson::CjsonElement, public tenduke::json::JSONObject
{
public:
    CjsonObject(
            std::unique_ptr<std::map<std::string, std::shared_ptr<tenduke::json::JSONElement>>> properties,
            cJSON * element
    ) : CjsonElement(JSONElement::Type::OBJECT, element), properties(std::move(properties))
    {}

    virtual ~CjsonObject();

    // Implementations
    virtual std::shared_ptr<tenduke::json::JSONElement> getProperty(const std::string &propertyName) const override;
    virtual bool hasProperty(const std::string &propertyName) const override;
    virtual std::shared_ptr<tenduke::json::JSONElement> removeProperty(const std::string & propertyName) override;
    virtual const std::map<std::string, std::shared_ptr<tenduke::json::JSONElement>> & getProperties() const override;

private:
    std::unique_ptr<std::map<std::string, std::shared_ptr<tenduke::json::JSONElement>>> properties;
};

}}}

#endif // CJSONOBJECT_H
