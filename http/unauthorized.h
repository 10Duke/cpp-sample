#ifndef UNAUTHORIZED_H
#define UNAUTHORIZED_H

#include "./clienterror.h"

namespace tenduke { namespace http {

/** Thrown for HTTP status code 401, "Unauthorized".
 *
 */
class Unauthorized : public tenduke::http::ClientError
{
public:
    Unauthorized(
            int httpStatusCode,
            const std::string &message
    )
        : tenduke::http::ClientError(httpStatusCode, message)
    {}
};

}}

#endif // UNAUTHORIZED_H
