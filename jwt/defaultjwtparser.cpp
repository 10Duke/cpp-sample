#include "./defaultjwtparser.h"
#include "./jwtparsingexception.h"
#include "./jwtvalidationexception.h"

#include "crypto/cryptoexception.h"
#include "json/jsonelement.h"
#include "json/jsonobject.h"
#include "json/jsonparsingexception.h"

#include <memory>

namespace crypto = tenduke::crypto;
namespace jwt = tenduke::jwt;
namespace json = tenduke::json;
namespace utl = tenduke::utl;


jwt::DefaultJWTParser::DefaultJWTParser(
        std::shared_ptr<const tenduke::json::JSONParser> jsonParser,
        std::shared_ptr<const tenduke::utl::Base64Decoder> base64Decoder,
        std::shared_ptr<const tenduke::jwt::JWTSignatureValidator> signatureValidator
) : parseJson(jsonParser), base64(base64Decoder), signature(signatureValidator)
{
}


jwt::JWT jwt::DefaultJWTParser::from(const std::string &serializedJWT) const
{
    // First locate the dots which separate the parts of the JWT:
    const std::string::size_type firstPeriod = serializedJWT.find('.');
    if (firstPeriod == std::string::npos) {
        throw jwt::JWTParsingException(0, "Not a JWT: Invalid structure");
    }
    if (firstPeriod == serializedJWT.size() - 1) {
        throw jwt::JWTParsingException(0, "Not a JWT: Invalid structure (only one period)");
    }

    const std::string::size_type secondPeriod = serializedJWT.find('.', firstPeriod + 1);
    if (secondPeriod == std::string::npos) {
        throw jwt::JWTParsingException(0, "Not a JWT: Invalid structure (only one period)");
    }

    const char * const data = serializedJWT.c_str();
    std::unique_ptr<json::JSONElement> header;

    // Parse header
    try {
        header = parseJson->from(base64->decodeUrlSafeString(data, firstPeriod));
    }
    catch (json::JSONParsingException &e) {
        std::throw_with_nested(jwt::JWTParsingException(0, "JWT header does not parse"));
    }

    // Validate header
    if (!header->isObject()) {
        throw jwt::JWTParsingException(0, "JWT header is not JSON object");
    }

    // Validate signature:
    if (signature != nullptr) {
        std::unique_ptr<utl::BinaryData> signatureBytes = base64->decodeUrlSafe(
                    data + secondPeriod + 1,
                    serializedJWT.size() - secondPeriod - 1
        );
        try {
            if (! signature->validate(
                dynamic_cast<const json::JSONObject *>(header.get()),
                data,
                secondPeriod,
                signatureBytes->getData(),
                signatureBytes->getLength()
            )) {
                throw jwt::JWTValidationException(0, "JWT signature does not validate");
            }
        }
        catch (const crypto::CryptoException &e) {
            // CryptoException should normally not occur, but lets wrap the exception anyway
            std::throw_with_nested(jwt::JWTValidationException(0, "JWT Signature validation failed due to crypto issue"));
        }
    }

    // Parse claims:
    std::unique_ptr<json::JSONElement> claimsElement;

    try {
        claimsElement = parseJson->from(base64->decodeUrlSafeString(
            data + firstPeriod + 1,
            secondPeriod - firstPeriod - 1
        ));
    }
    catch (json::JSONParsingException &e) {
        std::throw_with_nested(jwt::JWTParsingException(0, "JWT body does not parse"));
    }

    if (! claimsElement->isObject()) {
        throw jwt::JWTParsingException(0, "JWT claims is not an object");
    }

    const json::JSONObject * claimsObject = dynamic_cast<const json::JSONObject *>(claimsElement.get());
    std::map<std::string, std::string> claims;

    for (auto const &property : claimsObject->getProperties()) {
        claims.emplace(
            property.first,
            property.second->asString()
        );
    }

    return jwt::JWT(claims);
}


