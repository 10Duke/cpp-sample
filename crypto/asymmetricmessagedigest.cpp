#include "./asymmetricmessagedigest.h"

#include "./createlibcryptoexception.h"
#include "./cryptoexception.h"
#include "./publickey.h"

#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

namespace crypto = tenduke::crypto;
namespace libcrypto = tenduke::crypto::libcrypto;


libcrypto::AsymmetricMessageDigest::AsymmetricMessageDigest(
        const std::string &algorithmName
      , const std::shared_ptr<const crypto::PublicKey> verificationKey
)
    : algorithmName(algorithmName), verificationKey(verificationKey),
      digest(EVP_get_digestbyname(algorithmName.c_str())),
      ctx(EVP_MD_CTX_create(), EVP_MD_CTX_free)
{
    if (digest == nullptr) {
        throw crypto::CryptoException("digest_unsupported_algorithm", "Unsupported algorithm: " + algorithmName);
    }

    if (ctx == nullptr) {
        throw crypto::CryptoException("md_ctx_creation_failed", "Creation of libcrypto digest context failed");
    }
}


libcrypto::AsymmetricMessageDigest::~AsymmetricMessageDigest()
{
}

bool libcrypto::AsymmetricMessageDigest::verify(
        const void *message,
        const size_t messageLength,
        const void *signature,
        const size_t signatureLength
) {
    int status = EVP_DigestInit(ctx.get(), digest);
    if (status != 1) {
        throw libcrypto::createLibCryptoException(
            "digest_init_failed",
            "Libcrypto digest initialization failed"
        );
    }

    status = EVP_DigestVerifyInit(
                ctx.get(),
                NULL,
                digest,
                NULL,
                this->verificationKey->getKey()
    );
    if (status != 1) {
        throw libcrypto::createLibCryptoException(
            "digest_verify_init_failed",
            "Libcrypto digest verify initialization failed"
        );
    }

    status = EVP_DigestVerifyUpdate(ctx.get(), message, messageLength);
    if (status != 1) {
        throw libcrypto::createLibCryptoException(
            "digest_verify_update_failed",
            "Libcrypto digest verify update failed"
        );
    }

    ERR_clear_error();

    status = EVP_DigestVerifyFinal(
                ctx.get(),
                (unsigned char*)signature,
                signatureLength
    );

    // status 1 = signature is valid
    // status 0 = signature is invalid
    // status < 0 = some other error, e.g. invalid signature format
    if (status < 0) {
        throw libcrypto::createLibCryptoException(
            "md_digest_error",
            "Signature validation failed"
        );
    }

    return (status == 1);
}
