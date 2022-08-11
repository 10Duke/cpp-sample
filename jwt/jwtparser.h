#ifndef JWTPARSER_H
#define JWTPARSER_H

#include "./jwt.h"

#include <string>

namespace tenduke { namespace jwt {


/** Parses JWT.
 *
 */
class JWTParser
{
public:
    virtual ~JWTParser() {}

    /** Parses tenduke::jwt::JWT from given string.
     *
     *  @throws tenduke::jwt::JWTValidationException if validation of the JWT signature fails
     *  @throws tenduke::jwt::JWTParsingException (possibly some subclass) if parsing fails.
     *  @throws std::nested_exception if there is nested cause. Following nested causes are known:
     *          - tenduke::json::JSONParsingException when JSON parsing of the JWT parts fails
     *          - tenduke::crypto::CryptoException if there is some cryptographic-issue when validating the JWT signature
     */
    virtual JWT from(const std::string &value) const = 0;
};


}}

#endif // JWTPARSER_H
