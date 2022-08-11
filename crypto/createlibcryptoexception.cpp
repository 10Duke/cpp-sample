#include "createlibcryptoexception.h"

#include <openssl/err.h>

namespace crypto = tenduke::crypto;
namespace libcrypto = tenduke::crypto::libcrypto;

crypto::CryptoException libcrypto::createLibCryptoException (
    const std::string &error,
    const std::string &message
)
{
    unsigned long errorCode = ERR_get_error();
    char errorMessage[256];

    ERR_error_string_n(errorCode, errorMessage, sizeof(errorMessage));

    return crypto::CryptoException(
        error,
        message,
        errorCode,
        errorMessage
    );
}

