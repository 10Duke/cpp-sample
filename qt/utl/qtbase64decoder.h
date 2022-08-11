#ifndef TENDUKE_QT_UTL_QTBASE64DECODER_H
#define TENDUKE_QT_UTL_QTBASE64DECODER_H

#include <utl/base64decoder.h>

namespace tenduke { namespace qt { namespace utl {


/** QT-implementation of tenduke::utl::Base64Decoder.
 *
 */
class QtBase64Decoder : public tenduke::utl::Base64Decoder
{
    // tenduke::utl::Base64Decoder interface
public:
    virtual std::string decodeString(const std::string &base64EncodedString) const override;
    virtual std::string decodeString(
                const char *base64EncodedString,
                const size_t numBytes
    ) const override;

    virtual std::unique_ptr<tenduke::utl::BinaryData> decodeUrlSafe(
            const char *base64EncodedString,
            const size_t numBytes
    ) const override;
    virtual std::string decodeUrlSafeString(const std::string &base64EncodedString) const override;
    virtual std::string decodeUrlSafeString(
                const char *base64EncodedString,
                const size_t numBytes
    ) const override;
};


}}}

#endif // TENDUKE_QT_UTL_QTBASE64DECODER_H
