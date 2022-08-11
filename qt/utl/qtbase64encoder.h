#ifndef QTBASE64ENCODER_H
#define QTBASE64ENCODER_H

#include "utl/base64encoder.h"

namespace tenduke { namespace qt { namespace utl {


/** QT-implementation of tenduke::utl::Base64Encoder.
 *
 */
class QtBase64Encoder : public tenduke::utl::Base64Encoder
{
public:

    // tenduke::utl::Base64Encoder interface
public:
    std::string encode(const unsigned char * data, const size_t numBytes) const override;
    std::string encode(const std::string &string) const override;
    std::string encodeUrlSafe(const unsigned char *data, const size_t numBytes) const override;
    std::string encodeUrlSafe(const std::string &string) const override;
};


}}}

#endif // QTBASE64ENCODER_H
