#ifndef LIBCRYPTOMESSAGEDIGEST_H
#define LIBCRYPTOMESSAGEDIGEST_H

#include "./messagedigest.h"

#include <memory>
#include <openssl/evp.h>

namespace tenduke { namespace crypto { namespace libcrypto {


const char * const ALGORITHM_SHA256 = "sha256";


/** Libcrypto-implementation of tenduke::crypto::MessageDigest.
 *
 */
class LibCryptoMessageDigest : public MessageDigest
{
public:
    LibCryptoMessageDigest(const char * const algorithmName);
    virtual ~LibCryptoMessageDigest();

    virtual size_t length() override;
    virtual MessageDigest * update(
            const unsigned char * message,
            const size_t messageLength
    ) override;
    virtual void digestTo(unsigned char * digest) override;
    virtual std::unique_ptr<tenduke::utl::BinaryData> digest() override;

protected:
    LibCryptoMessageDigest(
            const char * const algorithmName,
            const EVP_MD *digest
    );

private:
    const EVP_MD * theDigest;
    const std::unique_ptr<EVP_MD_CTX, decltype(EVP_MD_CTX_free) *> ctx;
};


}}}

#endif // LIBCRYPTOMESSAGEDIGEST_H
