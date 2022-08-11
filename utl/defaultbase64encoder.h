#ifndef DEFAULTBASE64ENCODER_H
#define DEFAULTBASE64ENCODER_H

#include "base64encoder.h"

namespace tenduke { namespace utl {


/** Default tenduke::utl::Base64Encoder implementation.
 *
 */
class DefaultBase64Encoder : public tenduke::utl::Base64Encoder
{
public:
    virtual ~DefaultBase64Encoder() {}

// Base64Encoder interface
public:
    virtual std::string encode(const unsigned char *data, const size_t numBytes) const override;
    virtual std::string encode(const std::string &string) const override;
    virtual std::string encodeUrlSafe(const unsigned char *, const size_t numBytes) const override;
    virtual std::string encodeUrlSafe(const std::string &string) const override;
};


}}

#endif // DEFAULTBASE64ENCODER_H
