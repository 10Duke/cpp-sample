#ifndef BASE64ENCODER_H
#define BASE64ENCODER_H

#include <stddef.h>
#include <string>

namespace tenduke { namespace utl {


/** Base64-encoder.
 *
 */
class Base64Encoder
{
public:
    virtual ~Base64Encoder() {}

    virtual std::string encode(
                const unsigned char *,
                const size_t numBytes
    ) const = 0;
    virtual std::string encode(const std::string &string) const = 0;
    virtual std::string encodeUrlSafe(
                const unsigned char *,
                const size_t numBytes
    ) const = 0;
    virtual std::string encodeUrlSafe(const std::string &string) const = 0;
};


}}


#endif // BASE64ENCODER_H
