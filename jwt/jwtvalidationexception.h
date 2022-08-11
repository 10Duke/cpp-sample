#ifndef JWTVALIDATIONEXCEPTION_H
#define JWTVALIDATIONEXCEPTION_H

#include "./jwtparsingexception.h"
#include <string>

namespace tenduke { namespace jwt {


/** Thrown when JWT-validation failed (e.g. signature invalid).
 *
 */
class JWTValidationException : public JWTParsingException
{
public:
    JWTValidationException(const int code, const std::string &message)
        : JWTParsingException(code, message)
    {}

};

}}

#endif // JWTVALIDATIONEXCEPTION_H
