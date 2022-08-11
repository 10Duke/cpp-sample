#ifndef JSONOBJECT_H
#define JSONOBJECT_H

#include "./jsonelement.h"

#include <string>
#include <map>
#include <memory>

namespace tenduke { namespace json {

class JSONObject : public virtual JSONElement
{
public:
    virtual ~JSONObject() {}

    virtual std::shared_ptr<JSONElement> getProperty(const std::string & propertyName) const = 0;
    virtual bool hasProperty(const std::string & propertyName) const = 0;
    virtual std::shared_ptr<JSONElement> removeProperty(const std::string & propertyName) = 0;

    virtual const std::map<std::string, std::shared_ptr<JSONElement>> & getProperties() const = 0;
};

}}

#endif // JSONOBJECT_H
