#ifndef JSONBOOLEAN_H
#define JSONBOOLEAN_H

#include "./jsonelement.h"

namespace tenduke { namespace json {

class JSONBoolean : public virtual JSONElement
{
public:
    virtual ~JSONBoolean() {}

    virtual bool getValue() const = 0;
};

}}

#endif // JSONBOOLEAN_H
