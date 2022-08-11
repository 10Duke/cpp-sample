#ifndef DEFAULTBASE64DECODER_H
#define DEFAULTBASE64DECODER_H

#include "./base64decoder.h"

namespace tenduke { namespace utl {


/** Default tenduke::utl::Base64Decoder implementation.
 *
 *  **NOTE:** This implementation is quite lenient, e.g. it ignores unrecognized characters
 *  and allows mixed regular and URL-safe decoding.
 */
class DefaultBase64Decoder : public Base64Decoder
{
public:
    virtual ~DefaultBase64Decoder() {}

    virtual std::string decodeString(const std::string &base64EncodedString) const override;
    virtual std::string decodeString(
                const char *base64EncodedString,
                const size_t numBytes
    ) const override;

    virtual std::unique_ptr<BinaryData> decodeUrlSafe(
            const char *base64EncodedString,
            const size_t numBytes
    ) const override;
    virtual std::string decodeUrlSafeString(const std::string &base64EncodedString) const override;
    virtual std::string decodeUrlSafeString(
                const char *base64EncodedString,
                const size_t numBytes
    ) const override;
};


}}

#endif // DEFAULTBASE64DECODER_H
