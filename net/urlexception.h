#ifndef URLEXCEPTION_H
#define URLEXCEPTION_H

#include "./networkingexception.h"

namespace tenduke { namespace net {


/** A tenduke::net::NetworkingException thrown when various URL-related errors occur.
 *  Also a base class for more specific errors.
 *
 */
class URLException : public NetworkingException
{
public:
    URLException(const std::string& message)
        : tenduke::net::NetworkingException(message)
    {}
};


}}

#endif // URLEXCEPTION_H
