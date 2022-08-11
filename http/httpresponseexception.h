#ifndef HTTPRESPONSEEXCEPTION_H
#define HTTPRESPONSEEXCEPTION_H

#include "./httpexception.h"

namespace tenduke { namespace http {

/** This exception is thrown when HTTP-request fails because of HTTP-statuscode.
 *  This exception is usually thrown by users of tenduke::http::HTTPClient, the client
 *  itself does not care about the HTTP-status code.
 *
 */
class HTTPResponseException : public tenduke::http::HTTPException
{
public:
    HTTPResponseException(
            int httpStatusCode,
            const std::string &message
    ) : tenduke::http::HTTPException(httpStatusCode, message), statusCode(httpStatusCode)
    {}

    int getStatusCode() const {return statusCode;}

private:
    const int statusCode;
};

}}

#endif // HTTPRESPONSEEXCEPTION_H
