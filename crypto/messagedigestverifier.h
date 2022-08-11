#ifndef MESSAGEDIGESTVERIFIER_H
#define MESSAGEDIGESTVERIFIER_H

#include <stddef.h>

namespace tenduke { namespace crypto {


/** Verifies message by computing digest for message and comparing the computed signature to the provided signature.
 *
 *  NOTE: Instances are not thread safe.
 *
 *  NOTE: Most implementations throw tenduke::crypto::CryptoException if construction fails.
 *
 */
class MessageDigestVerifier
{
public:
    virtual ~MessageDigestVerifier() {}

    /** Verifies the digest.
     *
     *  @param message -
     *  @param messageLength -
     *  @param signature -
     *  @param signatureLength -
     *  @return true if computed message digest matches provided signature, false if not.
     *  @throws tenduke::crypto::CryptoException for various conditions.
     */
    virtual bool verify(
            const void * message,
            const size_t messageLength,
            const void * signature,
            const size_t signatureLength
    ) = 0;
};


}}

#endif // MESSAGEDIGESTVERIFIER_H
