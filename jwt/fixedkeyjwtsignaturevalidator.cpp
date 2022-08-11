#include "./fixedkeyjwtsignaturevalidator.h"

namespace jwt = tenduke::jwt;
namespace json = tenduke::json;

jwt::FixedKeyJWTSignatureValidator::FixedKeyJWTSignatureValidator(
        std::shared_ptr<const tenduke::crypto::MessageDigestVerifierFactory> verifierFactory
) : newDigest(verifierFactory)
{
}

bool jwt::FixedKeyJWTSignatureValidator::validate(
        const json::JSONObject *jwtHeader,
        const char * const payload,
        size_t payloadLength,
        const unsigned char * const signatureBytes,
        size_t signatureLengthB
) const
{
    return newDigest->create()->verify(
        payload,
        payloadLength,
        signatureBytes,
        signatureLengthB
    );
}
