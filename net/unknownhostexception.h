#ifndef UNKNOWNHOSTEXCEPTION_H
#define UNKNOWNHOSTEXCEPTION_H

#include "./networkingexception.h"

namespace tenduke { namespace net {


/** A tenduke::net::NetworkingException thrown when the host is not found.
 *
 *  In most applications this is usually configuration error and in those cases request is usually not re-tryable.
 */
class UnknownHostException : public NetworkingException
{
public:
    UnknownHostException(const std::string& message)
        : tenduke::net::NetworkingException(message)
    {}
};


}}

#endif // UNKNOWNHOSTEXCEPTION_H
