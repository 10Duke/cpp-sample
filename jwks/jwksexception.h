#ifndef TENDUKE_JWKS_JWKSEXCEPTION_H
#define TENDUKE_JWKS_JWKSEXCEPTION_H

#include <stdexcept>
#include <string>

namespace tenduke { namespace jwks {


/** Thrown when something goes with JWKS.
 *
 */
class JWKSException : public std::runtime_error
{
public:
    JWKSException(const std::string &message)
        : std::runtime_error(message)
    {}

    virtual ~JWKSException() {}
};


}}

#endif // TENDUKE_JWKS_JWKSEXCEPTION_H
