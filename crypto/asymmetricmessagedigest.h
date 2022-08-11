#ifndef ASYMMETRICMESSAGEDIGEST_H
#define ASYMMETRICMESSAGEDIGEST_H

#include "./messagedigestverifier.h"
#include "./publickey.h"

#include <memory>
#include <string>

#include <openssl/evp.h>

namespace tenduke { namespace crypto { namespace libcrypto {


/** Libcrypto-implementation of asymmetric tenduke::crypto::MessageDigestVerifier.
 *
 *  Instance is not thread-safe.
 *
 */
class AsymmetricMessageDigest : public MessageDigestVerifier
{
public:
    /** Constructs new instance.
     *
     * @param algorithmName -
     * @param publicKey -
     *
     * @throws tenduke::crypto::CryptoException e.g. if no such algorithm
     */
    AsymmetricMessageDigest(
            const std::string &algorithmName,
            const std::shared_ptr<const PublicKey> publicKey
    );
    virtual ~AsymmetricMessageDigest();

    /** Verifies the signature
     *
     * @param message -
     * @param messageLength -
     * @param signature -
     * @param signatureLength -
     * @return true if signature verifies
     *
     * @throws tenduke::crypto::CryptoException if libcrypto decides there is something wrong
     */
    virtual bool verify(
            const void *message,
            const size_t messageLength,
            const void *signature,
            const size_t signatureLength
    );

private:
    const std::string algorithmName;
    const std::shared_ptr<const PublicKey> verificationKey;
    const EVP_MD *digest;
    const std::unique_ptr<EVP_MD_CTX, decltype(EVP_MD_CTX_free) *> ctx;
};


}}}

#endif // ASYMMETRICMESSAGEDIGEST_H
