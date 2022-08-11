#ifndef CRYPTOEXCEPTION_H
#define CRYPTOEXCEPTION_H

#include <stdexcept>
#include <string>

namespace tenduke { namespace crypto {


/** Thrown when something goes with crypto.
 *
 */
class CryptoException : public std::runtime_error
{
public:
    CryptoException(
            const std::string &errorCode,
            const std::string &message,
            const long statusCode,
            const char * const technicalErrorMessage
    ) : std::runtime_error(message), errorCode(errorCode), statusCode(statusCode),
        technicalErrorMessage(technicalErrorMessage == nullptr ? "" : std::string(technicalErrorMessage))
    {}
    CryptoException(
            const std::string &errorCode,
            const std::string &message
    ) : CryptoException(errorCode, message, 0, nullptr)
    {}
    virtual ~CryptoException() {}

    const std::string errorCode;
    const long statusCode; // long because libcrypto uses longs for error codes...
    const std::string technicalErrorMessage; // Provided by e.g. library used
};


}}

#endif // CRYPTOEXCEPTION_H
