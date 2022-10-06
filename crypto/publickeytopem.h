#ifndef TENDUKE_CRYPTO_LIBCRYPTO_PUBLICKEYTOPEM_H
#define TENDUKE_CRYPTO_LIBCRYPTO_PUBLICKEYTOPEM_H

#include "./publickey.h"

#include <string>

namespace tenduke { namespace crypto { namespace libcrypto {

/** Utility service to convert Libcrypto public key (EVP_PKEY) to PEM.
 */
class PublicKeyToPEM
{
public:
    PublicKeyToPEM();

    std::string from(const PublicKey &publicKey) const;
};

}}}


#endif // TENDUKE_CRYPTO_LIBCRYPTO_PUBLICKEYTOPEM_H
