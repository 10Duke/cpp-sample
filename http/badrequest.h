#ifndef BADREQUEST_H
#define BADREQUEST_H

#include "./clienterror.h"

namespace tenduke { namespace http {

/** Thrown for HTTP status code 400 "BadRequest".
 *
 */
class BadRequest : public tenduke::http::ClientError
{
public:
    BadRequest(
            int httpStatusCode,
            const std::string &message
    )
        : tenduke::http::ClientError(httpStatusCode, message)
    {}
};

}}

#endif // BADREQUEST_H
