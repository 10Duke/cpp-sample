#ifndef OIDCEXCEPTION_H
#define OIDCEXCEPTION_H

#include "oauth/oauthexception.h"

namespace tenduke { namespace oauth { namespace oidc {


/** Thrown when OIDC-related errors occur.
 *
 */
class OIDCException : public tenduke::oauth::OAuthException
{
public:
    OIDCException(
            enum tenduke::oauth::OAuthException::Phase phase,
            const std::string &error,
            const std::string &errorDescription
    )
        : tenduke::oauth::OAuthException(phase, error, errorDescription)
    {}

    OIDCException(const std::string &errorDescription)
        : OIDCException(tenduke::oauth::OAuthException::Phase::NONE, "", errorDescription)
    {}

};


}}}

#endif // OIDCEXCEPTION_H
