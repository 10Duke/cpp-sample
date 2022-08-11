#ifndef ASYMMETRICMESSAGEDIGESTFACTORY_H
#define ASYMMETRICMESSAGEDIGESTFACTORY_H

#include "./messagedigestverifierfactory.h"
#include "./publickey.h"

#include <string>

namespace tenduke { namespace crypto { namespace libcrypto {


/** A tenduke::crypto::MessageDigestFactory for creating AsymmetricMessageDigest instances.
 *
 *  The factory is pre-configured with algorithm name and public key.
 *
 */
class AsymmetricMessageDigestFactory : public tenduke::crypto::MessageDigestVerifierFactory
{
public:
    AsymmetricMessageDigestFactory(
            const std::string &algorithmName,
            std::shared_ptr<const PublicKey> publicKey
    );

// tenduke::crypto::MessageDigestVerifierFactory interface
public:
    std::unique_ptr<MessageDigestVerifier> create() const;


private:
    const std::string algorithmName;
    const std::shared_ptr<const PublicKey> publicKey;
};


}}}

#endif // ASYMMETRICMESSAGEDIGESTFACTORY_H
