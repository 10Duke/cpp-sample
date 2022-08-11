#ifndef MESSAGEDIGESTVERIFIERFACTORY_H
#define MESSAGEDIGESTVERIFIERFACTORY_H

#include "./messagedigestverifier.h"

#include <memory>

namespace tenduke { namespace crypto {


/** Factory for creating pre-configured instances of tenduke::crypto::MessageDigestVerifier.
 *
 */
class MessageDigestVerifierFactory
{
public:
    virtual ~MessageDigestVerifierFactory() {}

    /** Creates the verifier.
     *
     *  @return new tenduke::crypto::MessageDigestVerifier instance
     */
    virtual std::unique_ptr<MessageDigestVerifier> create() const = 0;
};


}}

#endif // MESSAGEDIGESTVERIFIERFACTORY_H
