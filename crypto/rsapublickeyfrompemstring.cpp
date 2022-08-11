#include "./rsapublickeyfrompemstring.h"

#include "./cryptoexception.h"
#include "./createlibcryptoexception.h"

#include <memory>

#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

namespace crypto = tenduke::crypto;
namespace libcrypto = tenduke::crypto::libcrypto;

libcrypto::RSAPublicKeyFromPEMString::RSAPublicKeyFromPEMString()
{
}

std::unique_ptr<const crypto::PublicKey> libcrypto::RSAPublicKeyFromPEMString::from(const std::string &string)
{
    std::unique_ptr<BIO, decltype(&BIO_free)> buffer(BIO_new(BIO_s_mem()), &BIO_free);
    if (buffer == nullptr) {
        throw libcrypto::createLibCryptoException(
            "memory_error",
            "Unable allocate memory with BIO_new()"
        );
    }

    int status;
    size_t numBytesWritten;

    status = BIO_write_ex(buffer.get(), string.c_str(), string.size(), &numBytesWritten);
    if (status != 1) {
        throw libcrypto::createLibCryptoException(
            "initialization_error",
            "Error writing key to buffer with BIO_write_ex"
        );
    }

    RSA *rsaKeyPtr = NULL;
    if (PEM_read_bio_RSA_PUBKEY(buffer.get(), &rsaKeyPtr, NULL, NULL) == NULL) {
        throw libcrypto::createLibCryptoException(
            "key_error",
            "Error reading key from buffer with PEM_read_biod_RSA_PUBKEY"
        );
    }
    std::unique_ptr<RSA, decltype(&RSA_free)> rsaKey(rsaKeyPtr, &RSA_free);

    std::unique_ptr<EVP_PKEY, decltype(&EVP_PKEY_free)> publicKey(EVP_PKEY_new(), &EVP_PKEY_free);
    if(publicKey == nullptr) {
        throw libcrypto::createLibCryptoException(
              "initialization_error",
              "Error initializing key with EVP_PKEY_new()"
        );
    }

    status = EVP_PKEY_assign_RSA(publicKey.get(), RSAPublicKey_dup(rsaKey.get()));
    if(status != 1) {
        throw libcrypto::createLibCryptoException(
           "key_error",
           "Assigning key with EVP_PKEY_assign_RSA failed"
       );
    }

    return std::unique_ptr<const crypto::PublicKey>(new crypto::PublicKey(publicKey.release()));
}
