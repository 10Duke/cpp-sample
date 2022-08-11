#ifndef JWTSIGNATUREVALIDATOR_H
#define JWTSIGNATUREVALIDATOR_H

#include "json/jsonobject.h"

#include <string>

namespace tenduke { namespace jwt {


/** Service for validating the JWT-signature.
 *
 */
class JWTSignatureValidator
{
public:
    virtual ~JWTSignatureValidator() {}

    /** Validates JWT signature.
     *
     *  @param jwtHeader JWT-header, can be e.g. used to determine the key and algorithm used
     *  @param payload JWT-payload to verify
     *  @param payloadLength length of the payload
     *  @param signatureBytes the signature
     *  @param signatureLengthB the signature length
     *
     *  @return true if the signature is valid, false if not
     *
     *  @throws tenduke::crypto::CryptoException if there are are cryptographic issues
     */
    virtual bool validate(
            const tenduke::json::JSONObject *jwtHeader,
            const char * const payload,
            size_t payloadLength,
            const unsigned char * const signatureBytes,
            size_t signatureLengthB
    ) const = 0;
};


}}

#endif // JWTSIGNATUREVALIDATOR_H
