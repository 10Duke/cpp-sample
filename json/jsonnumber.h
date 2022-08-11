#ifndef JSONNUMBER_H
#define JSONNUMBER_H

#include "./jsonelement.h"

namespace tenduke { namespace json {

class JSONNumber : virtual public JSONElement
{
public:
    virtual ~JSONNumber() {}

    virtual double getValue() const  = 0;
};

}}

#endif // JSONNUMBER_H
