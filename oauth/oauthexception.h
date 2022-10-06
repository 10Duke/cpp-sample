#ifndef OAUTHEXCEPTION_H
#define OAUTHEXCEPTION_H

#include <stdexcept>
#include <string>

namespace tenduke { namespace oauth {


/** Generic OAuth-related exception.
 *  Also a base-class for more specific exceptions.
 */
class OAuthException : public std::runtime_error
{
public:
    enum Phase
    {
        NONE,
        AUTHORIZATION,
        TOKEN_REQUEST,
        REFRESH_REQUEST
    };

    OAuthException(
            Phase phase,
            const std::string &error,
            const std::string &getErrorDescription
    ) : std::runtime_error(getErrorDescription), phase(phase), error(error)
    {}

    Phase getPhase() const {return phase;}
    const std::string getError() const {return error;}
    const std::string getErrorDescription() const {return what();}

private:
    const Phase phase;
    const std::string error;
};


}}

#endif // OAUTHEXCEPTION_H
