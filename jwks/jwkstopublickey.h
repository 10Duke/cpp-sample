#ifndef TENDUKE_JWKS_JWKSTOKEY_H
#define TENDUKE_JWKS_JWKSTOKEY_H

#include "crypto/publickey.h"
#include "json/jsonobject.h"
#include "json/jsonparser.h"
#include "utl/base64decoder.h"

#include <memory>
#include <string>

namespace tenduke { namespace jwks {

class JWKSToPublicKey
{
public:
    JWKSToPublicKey(
            std::shared_ptr<const tenduke::json::JSONParser> jsonParser,
            std::shared_ptr<const tenduke::utl::Base64Decoder> base64Decoder
    );
    virtual ~JWKSToPublicKey();


    /** Extracts default signing key from JWKS-document.
     *
     *  This implementation assumes that the first key is the default.
     *
     *  @param jwksDocument the JWKS-document
     *  @return the extracted key
     *  @throws tenduke::crypto::CryptoException if problems with the signing key
     *  @throws tenduke::json::JSONParsingException when server response is not valid JSON
     *  @throws tenduke::jwks::JWKSException when the JWKS-document is valid JWKS.
     */
    virtual std::unique_ptr<const tenduke::crypto::PublicKey> from(const std::string &jwksDocument) const;

protected:
    /** Converts given JWKS in JSONObject to a PublicKey.
     *  This simple implementation only supports RSA keys.
     *
     *  @param key -
     *  @return -
     *  @throws tenduke::crypto::CryptoException if problems with the signing key
     *  @throws tenduke::json::JSONParsingException when server response is not valid JSON
     *  @throws tenduke::jwks::JWKSException when the JWKS-document is valid JWKS.
     */
    virtual std::unique_ptr<const tenduke::crypto::PublicKey> toPublicKey(const json::JSONObject *key) const;

private:
    std::shared_ptr<const tenduke::json::JSONParser> parseJSON;
    std::shared_ptr<const tenduke::utl::Base64Decoder> base64;
};

}}

#endif // TENDUKE_JWKS_JWKSTOKEY_H
