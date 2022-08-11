#ifndef CREATELIBCRYPTOEXCEPTION_H
#define CREATELIBCRYPTOEXCEPTION_H

#include "./cryptoexception.h"

#include <string>

namespace tenduke { namespace crypto { namespace libcrypto {


/** Utility function to create tenduke::crypto::CryptoException from libcrypto errors.
 *
 *  NOTE: Call this immediately after the error, this uses ERR_get_error() internally.
 */
crypto::CryptoException createLibCryptoException(
    const std::string &error,
    const std::string &messageBody
);


}}}

#endif // CREATELIBCRYPTOEXCEPTION_H
