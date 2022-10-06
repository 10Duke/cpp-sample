#include "./jwkstopublickey.h"
#include "./jwksexception.h"

#include "crypto/createlibcryptoexception.h"
#include "json/jsonarray.h"
#include "json/jsonelement.h"
#include "json/jsonobject.h"
#include "json/jsonutils.h"
#include "utl/binarydata.h"

#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>

namespace jwks = tenduke::jwks;
namespace crypto = tenduke::crypto;
namespace libcrypto = tenduke::crypto::libcrypto;
namespace json = tenduke::crypto;

jwks::JWKSToPublicKey::JWKSToPublicKey(
        std::shared_ptr<const tenduke::json::JSONParser> jsonParser,
        std::shared_ptr<const tenduke::utl::Base64Decoder> base64Decoder
) :
    parseJSON(jsonParser), base64(base64Decoder)
{}

jwks::JWKSToPublicKey::~JWKSToPublicKey()
{}


std::unique_ptr<const crypto::PublicKey> jwks::JWKSToPublicKey::from(const std::string &jwksDocument)
const
{
    std::unique_ptr<json::JSONElement> json = parseJSON->from(jwksDocument);

    // ERROR: Expecting payload to be JSON-object
    if (! json->isObject()) {
        throw JWKSException("JWKS-response is not JSON object");
    }

    const json::JSONObject* document = dynamic_cast<const json::JSONObject *>(json.get());

    // ERROR: The document does not have "keys"-property
    if (! document->hasProperty("keys")) {
        throw JWKSException("The JWKS-response does not have \"keys\"-property");
    }

    const std::shared_ptr<json::JSONElement> keysElement = document->getProperty("keys");

    // ERROR: Expecting "keys" to be an array
    if (! keysElement->isArray()) {
        throw JWKSException("The \"keys\"-property is not JSON array");
    }

    const json::JSONArray *keys = dynamic_cast<const json::JSONArray *>(keysElement.get());

    // ERROR: No entries in "keys"
    if (keys->isEmpty()) {
        throw JWKSException("No entries in the \"keys\" -array");
    }

    // This simple implementation just takes the first key:
    const std::shared_ptr<json::JSONElement> keyElement = keys->getValues().at(0);

    // ERROR: The key is not an object
    if (! keyElement->isObject()) {
        throw JWKSException("The key is not an object");
    }

    const json::JSONObject *key = dynamic_cast<const json::JSONObject *>(keyElement.get());

    return toPublicKey(key);
}


std::unique_ptr<const crypto::PublicKey> jwks::JWKSToPublicKey::toPublicKey(const json::JSONObject * key)
const
{
    // ERROR: This implementation only supports RSA keys
    std::string keyType = json::getObjectPropertyAsString(key, "kty");
    if (keyType != "RSA") {
        throw JWKSException("This implementation only supports RSA-keys (got \"" + keyType + "\")");
    }

    // ERROR: RSA key needs exponent and modulo
    if (! (key->hasProperty("n") && key->hasProperty("e"))) {
        throw JWKSException("The key is missing \"n\" or \"e\" property");
    }

    std::unique_ptr<utl::BinaryData> exponent = base64->decodeUrlSafe(key->getProperty("e")->asString());
    std::unique_ptr<BIGNUM, decltype(&BN_free)> exponentBN (BN_bin2bn(exponent->getData(), exponent->getLength(), NULL), &BN_free);
    if (exponentBN == nullptr) {
        throw libcrypto::createLibCryptoException(
              "initialization_error",
              "Internal error on building the key (BN_bin2bn in exponent)"
        );
    }

    std::unique_ptr<utl::BinaryData> modulo = base64->decodeUrlSafe(key->getProperty("n")->asString());
    std::unique_ptr<BIGNUM, decltype(&BN_free)> moduloBN (BN_bin2bn(modulo->getData(), modulo->getLength(), NULL), &BN_free);
    if (moduloBN == nullptr){
        throw libcrypto::createLibCryptoException(
              "initialization_error",
              "Internal error on building the key (BN_bin2bn in modulo)"
        );
    }

    std::unique_ptr<RSA, decltype(&RSA_free)> rsaKey(RSA_new(), &RSA_free);
    if (rsaKey == nullptr) {
        throw libcrypto::createLibCryptoException(
              "initialization_error",
              "Error initializing key with RSA_new()"
        );
    }

    // IMPORTANT: This moves the ownership of `moduloBN` and `exponentBN` to the key.
    // The key will maintain the memory for these objects.
    if (0 == RSA_set0_key(rsaKey.get(), moduloBN.release(), exponentBN.release(), NULL)) {
        throw libcrypto::createLibCryptoException(
              "initialization_error",
              "RSA key creation from modulo and exponent (RSA_set0_key()) failed"
        );
    }

    std::unique_ptr<EVP_PKEY, decltype(&EVP_PKEY_free)> publicKey(EVP_PKEY_new(), &EVP_PKEY_free);
    if(publicKey == nullptr) {
        throw libcrypto::createLibCryptoException(
              "initialization_error",
              "Error initializing key with EVP_PKEY_new()"
        );
    }

    if (0 == EVP_PKEY_assign_RSA(publicKey.get(), RSAPublicKey_dup(rsaKey.get()))) {
        throw libcrypto::createLibCryptoException(
           "key_error",
           "Assigning key with EVP_PKEY_assign_RSA failed"
       );
    }

    return std::unique_ptr<const crypto::PublicKey>(new crypto::PublicKey(publicKey.release()));
}
