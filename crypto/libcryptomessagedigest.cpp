#include "./libcryptomessagedigest.h"

#include "./createlibcryptoexception.h"
#include "utl/fixedsizebinarydata.h"

#include <string>


namespace crypto = tenduke::crypto;
namespace libcrypto = tenduke::crypto::libcrypto;
namespace utl = tenduke::utl;

libcrypto::LibCryptoMessageDigest::LibCryptoMessageDigest(
    const char * const  algorithmName,
    const EVP_MD *digest
)
    : theDigest(digest), ctx(EVP_MD_CTX_new(), EVP_MD_CTX_free)
{
    if (digest == NULL) {
        std::string message = "Unsupported algorithm" + std::string(algorithmName);
        throw crypto::CryptoException("digest_unsupported_algorithm", message);
    }

    if (ctx == nullptr) {
        throw crypto::CryptoException("md_ctx_creation_failed", "Creation of libcrypto digest context failed");
    }

    if (EVP_DigestInit_ex(ctx.get(), theDigest, NULL) != 1) {
        throw libcrypto::createLibCryptoException(
            "digest_init_failed",
            "Libcrypto digest initialization failed"
        );
    }
}

libcrypto::LibCryptoMessageDigest::LibCryptoMessageDigest(const char * const algorithmName)
    : libcrypto::LibCryptoMessageDigest(algorithmName, EVP_get_digestbyname(algorithmName))
{
}

libcrypto::LibCryptoMessageDigest::~LibCryptoMessageDigest()
{
}

size_t libcrypto::LibCryptoMessageDigest::length()
{
    return EVP_MD_CTX_size(ctx.get());
}

crypto::MessageDigest * libcrypto::LibCryptoMessageDigest::update(
    const unsigned char * message,
    const size_t messageLength
)
{
    int status = EVP_DigestUpdate(ctx.get(), message, messageLength);

    if (status != 1) {
        throw libcrypto::createLibCryptoException(
            "digest_update_failed",
            "Libcrypto digest update failed"
        );
    }

    return this;
}



void libcrypto::LibCryptoMessageDigest::digestTo(unsigned char * digest)
{
    int status = EVP_DigestFinal_ex(ctx.get(), digest, NULL);

    if (status != 1) {
        throw libcrypto::createLibCryptoException(
            "digest_finale_failed",
            "Digest computation failed"
        );
    }
}


std::unique_ptr<utl::BinaryData> libcrypto::LibCryptoMessageDigest::digest()
{
    size_t digestLength = length();
    std::unique_ptr<utl::BinaryData> messageDigest(new utl::FixedSizeBinaryData(digestLength));

    digestTo(messageDigest->getData());

    return messageDigest;
}
