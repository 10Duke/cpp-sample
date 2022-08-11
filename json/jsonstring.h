#ifndef JSONSTRING_H
#define JSONSTRING_H

#include "./jsonelement.h"

#include <string>

namespace tenduke { namespace json {

class JSONString : virtual public JSONElement
{
public:
    virtual ~JSONString() {}

    virtual const std::string & getValue() const = 0;
};

}}

#endif // JSONSTRING_H
