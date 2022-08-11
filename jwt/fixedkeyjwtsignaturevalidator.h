#ifndef FIXEDKEYJWTSIGNATUREVALIDATOR_H
#define FIXEDKEYJWTSIGNATUREVALIDATOR_H

#include "./jwtsignaturevalidator.h"

#include "crypto/messagedigestverifier.h"
#include "crypto/messagedigestverifierfactory.h"

namespace tenduke { namespace jwt {

/** A tenduke::jwt::JWTSignatureValidator, which expects that the signature algorithm and verification key
 *  are pre-configured.
 *
 *  The class uses tenduke::crypto::MessageDigestFactory to create the actual validator.
 *
 */
class FixedKeyJWTSignatureValidator : public tenduke::jwt::JWTSignatureValidator
{
public:
    FixedKeyJWTSignatureValidator(
        std::shared_ptr<const tenduke::crypto::MessageDigestVerifierFactory> verifierFactory
    );

// JWTSignatureValidator interface
public:
    virtual bool validate(
            const json::JSONObject *jwtHeader,
            const char * const payload,
            size_t payloadLength,
            const unsigned char * const signatureBytes,
            size_t signatureLengthB
    ) const override;

private:
    const std::shared_ptr<const tenduke::crypto::MessageDigestVerifierFactory> newDigest;
};

}}

#endif // FIXEDKEYJWTSIGNATUREVALIDATOR_H
