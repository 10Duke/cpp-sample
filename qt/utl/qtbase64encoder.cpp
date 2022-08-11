#include "qtbase64encoder.h"

#include <QByteArray>
#include <QString>

namespace qtutl = tenduke::qt::utl;

std::string qtutl::QtBase64Encoder::encode(const unsigned char *data, const size_t numBytes) const
{
    QByteArray base64EncodedBytes = QByteArray::fromRawData((char *)data, numBytes)
                .toBase64();

    return base64EncodedBytes.toStdString();
}

std::string qtutl::QtBase64Encoder::encode(const std::string &string) const
{
    QByteArray base64EncodedBytes = QByteArray::fromStdString(string)
                .toBase64();

    return base64EncodedBytes.toStdString();

}

std::string qtutl::QtBase64Encoder::encodeUrlSafe(const unsigned char *data, const size_t numBytes) const
{
    QByteArray base64EncodedBytes = QByteArray::fromRawData((char *)data, numBytes)
                .toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals);

    return base64EncodedBytes.toStdString();
}

std::string qtutl::QtBase64Encoder::encodeUrlSafe(const std::string &string) const
{
    QByteArray base64EncodedBytes = QByteArray::fromStdString(string)
                .toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals);

    return base64EncodedBytes.toStdString();
}
