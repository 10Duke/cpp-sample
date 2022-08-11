#include "./sha256utl.h"

#include "./messagedigest.h"
#include "./libcryptomessagedigest.h"

#include <memory>
#include <cstring>

namespace crypto = tenduke::crypto;
namespace libcrypto = tenduke::crypto::libcrypto;

void crypto::calculateSHA256(
        const unsigned char * fromBytes,
        const size_t numBytes,
        unsigned char * resultingHash
) {
    std::unique_ptr<crypto::MessageDigest> digest (new libcrypto::LibCryptoMessageDigest(libcrypto::ALGORITHM_SHA256));

    digest
            ->update(fromBytes, numBytes)
            ->digestTo(resultingHash);
}


std::unique_ptr<tenduke::utl::BinaryData> crypto::calculateSHA256(
        const unsigned char * fromBytes,
        const size_t numBytes
) {
    std::unique_ptr<crypto::MessageDigest> digest (new libcrypto::LibCryptoMessageDigest(libcrypto::ALGORITHM_SHA256));

    return digest
            ->update(fromBytes, numBytes)
            ->digest();
}
