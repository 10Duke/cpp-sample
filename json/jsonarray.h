#ifndef JSONARRAY_H
#define JSONARRAY_H

#include "./jsonelement.h"

#include <memory>
#include <stddef.h>
#include <vector>

namespace tenduke { namespace json {

class JSONArray : public virtual JSONElement
{
public:
    virtual ~JSONArray() {}

    virtual bool isEmpty() const = 0;
    virtual size_t getSize() const = 0;
    virtual const std::vector<std::shared_ptr<JSONElement>> & getValues() const = 0;
};

}}

#endif // JSONARRAY_H
