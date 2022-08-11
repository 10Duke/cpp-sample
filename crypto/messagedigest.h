#ifndef MESSAGEDIGEST_H
#define MESSAGEDIGEST_H

#include "utl/binarydata.h"
#include <memory>
#include <stddef.h>

namespace tenduke { namespace crypto {


/** Simple MessageDigest -service.
 *
 *  Instances are not thread-safe.
 *
 *  Construction may throw tenduke::crypto::CryptoException.
 */
class MessageDigest
{
public:
    virtual ~MessageDigest() {}

    /** Returns length (in bytes) of the resulting digest.
     *
     *  @return -
     */
    virtual size_t length() = 0;

    /** Updates the digest.
     *
     *  @param message -
     *  @param messageLength -
     *  @return this instance for chaining
     *  @throws tenduke::crypto::CryptoException on error
     */
    virtual MessageDigest * update(
            const unsigned char * message,
            const size_t messageLength
    ) = 0;

    /** Computes the digest to pre-allocated buffer.
     *
     *  @param messageDigest the computed digest is placed here (needs to proper size, use ::length()).
     *  @throws tenduke::crypto::CryptoException on error
     */
    virtual void digestTo(unsigned char * messageDigest) = 0;

    /** Computes the digest.
     *
     *  @return data-object containing the digest
     *  @throws tenduke::crypto::CryptoException on error
     */
    virtual std::unique_ptr<tenduke::utl::BinaryData> digest() = 0;
};


}}

#endif // MESSAGEDIGEST_H
