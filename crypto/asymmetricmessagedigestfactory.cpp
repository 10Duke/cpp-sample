#include "./asymmetricmessagedigestfactory.h"

#include "./asymmetricmessagedigest.h"

namespace crypto = tenduke::crypto;
namespace libcrypto = tenduke::crypto::libcrypto;

libcrypto::AsymmetricMessageDigestFactory::AsymmetricMessageDigestFactory(
        const std::string &algorithmName,
        std::shared_ptr<const crypto::PublicKey> publicKey
) : algorithmName(algorithmName), publicKey(publicKey)
{
}

std::unique_ptr<crypto::MessageDigestVerifier> libcrypto::AsymmetricMessageDigestFactory::create() const
{
    return std::unique_ptr<crypto::MessageDigestVerifier>(new libcrypto::AsymmetricMessageDigest(
        algorithmName,
        publicKey
    ));
}
