#ifndef FORBIDDEN_H
#define FORBIDDEN_H

#include "./clienterror.h"

namespace tenduke { namespace http {


/** Thrown for HTTP status code 403, "Forbidden".
 *
 */
class Forbidden : public tenduke::http::ClientError
{
public:
    Forbidden(
            int httpStatusCode,
            const std::string &message
    )
        : tenduke::http::ClientError(httpStatusCode, message)
    {}
};


}}

#endif // FORBIDDEN_H
