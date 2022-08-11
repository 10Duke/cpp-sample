#ifndef SHA256UTL_H
#define SHA256UTL_H

#include "utl/binarydata.h"
#include <memory>
#include <stddef.h>

namespace tenduke { namespace crypto {

// Length of SH256-hash in bytes. For those want to allocate the buffer for the hash themselves.
const size_t SHA256_HASH_LENGTH_BYTES = 32;


/** Utility-function to compute SHA256-hash.
 *
 *  @param fromBytes message data
 *  @param numBytes size (in bytes) of message data
 *  @param resultingHash the SHA256-hash is placed here. The pointer must be valid an proper size.
 *
 *  @throws tenduke::crypto::CryptoException if some libcrypto error
 */
void calculateSHA256(
        const unsigned char * fromBytes,
        const size_t numBytes,
        unsigned char * resultingHash
);


/** Utility-function to compute SHA256-hash.
 *
 *  @param fromBytes message data
 *  @param numBytes size (in bytes) of message data
 *
 *  @return the hash
 *
 *  @throws tenduke::crypto::CryptoException if some libcrypto error
 */
std::unique_ptr<tenduke::utl::BinaryData> calculateSHA256(
        const unsigned char * fromBytes,
        const size_t numBytes
);


}}

#endif // SHA256UTL_H
