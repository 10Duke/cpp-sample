#ifndef BASE64DECODER_H
#define BASE64DECODER_H

#include "utl/binarydata.h"

#include <memory>
#include <stddef.h>
#include <string>

namespace tenduke { namespace utl {


/** Base64-decoder.
 *
 */
class Base64Decoder
{
public:
    virtual ~Base64Decoder() {}

    virtual std::unique_ptr<BinaryData> decode(const std::string &base64EncodedString) const = 0;
    virtual std::unique_ptr<BinaryData> decode(
            const char *base64EncodedString,
            const size_t numBytes
    ) const = 0;
    virtual std::string decodeString(const std::string &base64EncodedString) const = 0;
    virtual std::string decodeString(
                const char *base64EncodedString,
                const size_t numBytes
    ) const = 0;

// URL-safe decoding
    /** Decodes given URL-safe Base64-encoded string to BinaryData.
     *
     *  @param base64EncodedString -
     *  @return -
     */
    virtual std::unique_ptr<BinaryData> decodeUrlSafe(const std::string &base64EncodedString) const = 0;

    /** Decodes given URL-safe Base64-encoded string to BinaryData.
     *
     *  @param base64EncodedString -
     *  @param numBytes -
     *  @return -
     */
    virtual std::unique_ptr<BinaryData> decodeUrlSafe(
            const char *base64EncodedString,
            const size_t numBytes
    ) const = 0;
    virtual std::string decodeUrlSafeString(const std::string &base64EncodedString) const = 0;
    virtual std::string decodeUrlSafeString(
                const char *base64EncodedString,
                const size_t numBytes
    ) const = 0;
};


}}


#endif // BASE64ENCODER_H
