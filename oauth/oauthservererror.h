#ifndef OAUTHSERVERERROR_H
#define OAUTHSERVERERROR_H

#include "./oauthexception.h"

#include <stdexcept>
#include <string>

namespace tenduke { namespace oauth {


/** Thrown when server reported an OAuth-related error.
 *
 */
class OAuthServerError : public tenduke::oauth::OAuthException
{
public:
    OAuthServerError(Phase phase, const std::string error, const std::string errorDescription)
        : tenduke::oauth::OAuthException(phase, error, errorDescription)
    {}
};


}}

#endif // OAUTHSERVERERROR_H
