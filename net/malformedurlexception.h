#ifndef MALFORMEDURLEXCEPTION_H
#define MALFORMEDURLEXCEPTION_H

#include "./urlexception.h"

namespace tenduke { namespace net {


/** Thrown when URL is malformed.
 */
class MalformedURLException : public tenduke::net::URLException
{
public:
    MalformedURLException(const std::string& message)
        : tenduke::net::URLException(message)
    {}
};


}}

#endif // MALFORMEDURLEXCEPTION_H
