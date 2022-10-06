#include "./qtbase64decoder.h"

#include "utl/fixedsizebinarydata.h"

#include <cstring>
#include <QByteArray>
#include <QString>

namespace xdutl = tenduke::utl;
namespace qtutl = tenduke::qt::utl;

std::unique_ptr<xdutl::BinaryData> qtutl::QtBase64Decoder::decode(const std::string &base64EncodedString)
const
{
    return decode(base64EncodedString.c_str(), base64EncodedString.size());
}


std::unique_ptr<xdutl::BinaryData> qtutl::QtBase64Decoder::decode(
        const char *base64EncodedString,
        const size_t numBytes
) const
{
    QByteArray data = QByteArray::fromBase64(QString::fromUtf8(base64EncodedString, numBytes).toUtf8());

    std::unique_ptr<unsigned char> bytes (new unsigned char[data.size()]);
    std::memcpy(bytes.get(), data.data(), data.size());

    return std::unique_ptr<xdutl::BinaryData>(new xdutl::FixedSizeBinaryData(std::move(bytes), data.size()));
}


std::string qtutl::QtBase64Decoder::decodeString(const std::string &base64EncodedString) const
{
    return QByteArray::fromBase64(QString::fromStdString(base64EncodedString).toUtf8()).toStdString();
}


std::string qtutl::QtBase64Decoder::decodeString(const char *base64EncodedString, const size_t numBytes) const
{
    return QByteArray::fromBase64(QString::fromUtf8(base64EncodedString, numBytes).toUtf8()).toStdString();
}


std::unique_ptr<xdutl::BinaryData> qtutl::QtBase64Decoder::decodeUrlSafe(const std::string &base64EncodedString)
const
{
    return decodeUrlSafe(base64EncodedString.c_str(), base64EncodedString.size());
}


std::unique_ptr<xdutl::BinaryData> qtutl::QtBase64Decoder::decodeUrlSafe(const char *base64EncodedString, const size_t numBytes) const
{
    QByteArray data = QByteArray::fromBase64(
                QString::fromUtf8(base64EncodedString, numBytes).toUtf8(),
                QByteArray::Base64Option::Base64UrlEncoding
    );

    std::unique_ptr<unsigned char> bytes (new unsigned char[data.size()]);
    std::memcpy(bytes.get(), data.data(), data.size());

    return std::unique_ptr<xdutl::BinaryData>(new xdutl::FixedSizeBinaryData(std::move(bytes), data.size()));
}


std::string qtutl::QtBase64Decoder::decodeUrlSafeString(const std::string &base64EncodedString) const
{
    return QByteArray::fromBase64(
        QString::fromStdString(base64EncodedString).toUtf8(),
        QByteArray::Base64Option::Base64UrlEncoding
    ).toStdString();
}


std::string qtutl::QtBase64Decoder::decodeUrlSafeString(const char *base64EncodedString, const size_t numBytes) const
{
    return QByteArray::fromBase64(
        QString::fromUtf8(base64EncodedString, numBytes).toUtf8(),
        QByteArray::Base64Option::Base64UrlEncoding
    ).toStdString();
}
