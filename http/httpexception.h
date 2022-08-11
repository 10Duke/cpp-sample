#ifndef HTTPEXCEPTION_H
#define HTTPEXCEPTION_H

#include "net/networkingexception.h"

namespace tenduke { namespace http {


/** A generic HTTP-exception.
 *  Also acts as a base class for more specific exceptions, especially:
 *  - tenduke::http::HTTPRequestException (and subclasses)
 *  - tenduke::http::HTTPResponseException (and subclasses)
 *
 */
class HTTPException : public tenduke::net::NetworkingException
{
public:
    HTTPException(
            int statusCode,
            const std::string &message
    ) : tenduke::net::NetworkingException(message), statusCode(statusCode)
    {}

    int getStatusCode() const {return statusCode;}

private:
    const int statusCode;
};


}}

#endif // HTTPEXCEPTION_H
