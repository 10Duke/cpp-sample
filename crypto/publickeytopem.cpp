#include "publickeytopem.h"

#include "./createlibcryptoexception.h"

#include <memory>

#include <openssl/bio.h>
#include <openssl/pem.h>

namespace xdlibcrypto = tenduke::crypto::libcrypto;

static const int ERROR = 0;

xdlibcrypto::PublicKeyToPEM::PublicKeyToPEM()
{
}

std::string xdlibcrypto::PublicKeyToPEM::from(const PublicKey &publicKey)
const
{
    std::unique_ptr<BIO, decltype(&BIO_free)> buffer(BIO_new(BIO_s_mem()), &BIO_free);
    if (buffer == nullptr) {
        throw libcrypto::createLibCryptoException(
            "memory_error",
            "Unable allocate memory with BIO_new()"
        );
    }

    int status = PEM_write_bio_PUBKEY(buffer.get(), publicKey.getKey());
    if (status == ERROR) {
        throw libcrypto::createLibCryptoException(
            "key_error",
            "Error writing key tobuffer with PEM_write_bio_PUBKEY"
        );
    }

    char *pemData;
    long numBytes = BIO_get_mem_data(buffer.get(), &pemData);

    if (numBytes == 0L) {
        throw libcrypto::createLibCryptoException(
            "key_error",
            "Data is NULL?!?!?"
        );
    }
    else if (numBytes < 0L) {
        throw libcrypto::createLibCryptoException(
            "key_error",
            "Error getting the PEM using BIO_get_mem_data"
        );
    }

    return std::string(pemData, numBytes);
}

