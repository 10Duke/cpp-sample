#ifndef CLIENTERROR_H
#define CLIENTERROR_H

#include "./httpresponseexception.h"

namespace tenduke { namespace http {

/** 4xx-series, base class for many 4xx-exceptions.
 *  Also used standalone for common catch-all 4xx-statuscodes.
 *
 */
class ClientError : public tenduke::http::HTTPResponseException
{
public:
    ClientError(
            int httpStatusCode,
            const std::string &message
    )
        : tenduke::http::HTTPResponseException(httpStatusCode, message)
    {}
};

}}

#endif // CLIENTERROR_H
