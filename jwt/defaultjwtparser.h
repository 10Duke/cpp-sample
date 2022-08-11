#ifndef DEFAULTJWTPARSER_H
#define DEFAULTJWTPARSER_H

#include "./jwtparser.h"
#include "./jwtsignaturevalidator.h"
#include "json/jsonparser.h"
#include "utl/base64decoder.h"

#include <memory>

namespace tenduke { namespace jwt {

/** Default implementation of tenduke::jwt::JWTParser.
 *
 */
class DefaultJWTParser : public tenduke::jwt::JWTParser
{
public:
    DefaultJWTParser(
            std::shared_ptr<const tenduke::json::JSONParser> jsonParser,
            std::shared_ptr<const tenduke::utl::Base64Decoder> base64Decoder,
            std::shared_ptr<const tenduke::jwt::JWTSignatureValidator> signatureValidator
    );

// JWTParser interface
public:
    virtual tenduke::jwt::JWT from(const std::string &value) const;

private:
    const std::shared_ptr<const tenduke::json::JSONParser> parseJson;
    const std::shared_ptr<const tenduke::utl::Base64Decoder> base64;
    const std::shared_ptr<const tenduke::jwt::JWTSignatureValidator> signature;
};

}}

#endif // DEFAULTJWTPARSER_H
