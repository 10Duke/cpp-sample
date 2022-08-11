#ifndef HTTPREQUESTEXCEPTION_H
#define HTTPREQUESTEXCEPTION_H

#include "net/networkingexception.h"

namespace tenduke { namespace http {


/** This exception is thrown when a HTTP-request fails for HTTP-specific reasons in the client end.
 *  Some possible causes for this exception are:
 *  - Invalid HTTP method
 *  - Malformed URL
 *
 *  This exception is **NOT** thrown when the call technically succeeds, but server responds with
 *  status code >= 400. The higher-level clients should handle those conditions and act accordingly.
 *  (Clients can use tenduke::http::HTTPResponseException and its subclasses for those conditions).
 */
class HTTPRequestException : public tenduke::net::NetworkingException
{
public:
    HTTPRequestException(const std::string &message) : tenduke::net::NetworkingException(message)
    {}
};


}}

#endif // HTTPREQUESTEXCEPTION_H
