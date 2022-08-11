#ifndef NOTFOUND_H
#define NOTFOUND_H

#include "./clienterror.h"

namespace tenduke { namespace http {

/** Thrown for HTTP status code 404, "Forbidden".
 *
 */
class NotFound : public tenduke::http::ClientError
{
public:
    NotFound(
            int httpStatusCode,
            const std::string &message
    )
        : tenduke::http::ClientError(httpStatusCode, message)
    {}
};

}}

#endif // NOTFOUND_H
