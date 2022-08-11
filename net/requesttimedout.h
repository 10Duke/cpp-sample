#ifndef REQUESTTIMEDOUT_H
#define REQUESTTIMEDOUT_H

#include "./networkingexception.h"

namespace tenduke { namespace net {


/** A tenduke::net::NetworkingException thrown when network request timed out.
 *
 */
class RequestTimedOut : public tenduke::net::NetworkingException
{
public:
    RequestTimedOut(const std::string& message)
        : tenduke::net::NetworkingException(message)
    {}
};


}}

#endif // REQUESTTIMEDOUT_H
