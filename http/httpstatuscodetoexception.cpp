#include "./httpstatuscodetoexception.h"

#include "./badrequest.h"
#include "./clienterror.h"
#include "./forbidden.h"
#include "./notfound.h"
#include "./servererror.h"
#include "./unauthorized.h"

namespace xdhttp = tenduke::http;

void xdhttp::HTTPStatusCodeToException::fromHTTPStatus(int httpStatusCode)
const
{
    if (httpStatusCode < 500) {
        from4xx(httpStatusCode);
    }
    return from5xx(httpStatusCode);
}

void xdhttp::HTTPStatusCodeToException::from4xx(int httpStatusCode)
const
{
    switch(httpStatusCode)
    {
        case 400:
            return from400();
        case 401:
            return from401();
        case 403:
            return from403();
        case 404:
            return from404();
        default:
            return from4xxOther(httpStatusCode);
    }
}

void xdhttp::HTTPStatusCodeToException::from400()
const
{
    throw xdhttp::BadRequest(
        400,
        "Bad request"
    );
}

void xdhttp::HTTPStatusCodeToException::from401()
const
{
    throw xdhttp::Unauthorized(
        401,
        "Unauthorized"
    );
}

void xdhttp::HTTPStatusCodeToException::from403()
const
{
    throw xdhttp::Forbidden(
        403,
        "Forbidden"
    );
}

void xdhttp::HTTPStatusCodeToException::from404()
const
{
    throw xdhttp::NotFound(
        404,
        "Not Found"
    );
}

void xdhttp::HTTPStatusCodeToException::from4xxOther(int httpStatusCode)
const
{
    throw xdhttp::ClientError(
        httpStatusCode,
        "Client error (HTTP status code: " + std::to_string(httpStatusCode) + ")"
    );
}

void xdhttp::HTTPStatusCodeToException::from5xx(int httpStatusCode)
const
{
    throw xdhttp::ServerError(
        httpStatusCode,
        "Server error (HTTP status code: " + std::to_string(httpStatusCode) + ")"
    );
}
