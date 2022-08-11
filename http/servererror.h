#ifndef SERVERERROR_H
#define SERVERERROR_H

#include "./httpresponseexception.h"

namespace tenduke { namespace http {

/** 5xx-error, base class for 5xx-exceptions.
 *  Also used standalone.
 */
class ServerError: public tenduke::http::HTTPResponseException
{
public:
    ServerError(
            int httpStatusCode,
            const std::string &message
    )
        : tenduke::http::HTTPResponseException(httpStatusCode, message)
    {}
};

}}

#endif // SERVERERROR_H
