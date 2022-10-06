#include "./defaultbase64decoder.h"
#include "./fixedsizebinarydata.h"

#include <string.h>

namespace utl = tenduke::utl;

// Implementation from:
// https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c/37109258#37109258
// https://stackoverflow.com/questions/342409/how-do-i-base64-encode-decode-in-c

static const int B64index[256] =
{
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  62, 63, 62, 62, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0,  0,  0,  0,  0,  0,
    0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0,  0,  0,  0,  63,
    0,  26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
};


std::unique_ptr<utl::BinaryData> utl::DefaultBase64Decoder::decode(
        const char *base64EncodedString,
        const size_t len
) const
{
    if (len == 0) {
        return std::unique_ptr<utl::BinaryData>(new utl::FixedSizeBinaryData(0));
    }

    unsigned char *p = (unsigned char*) base64EncodedString;
    size_t j = 0,
        pad1 = len % 4 || p[len - 1] == '=',
        pad2 = pad1 && (len % 4 > 2 || p[len - 2] != '=');
    const size_t last = (len - pad1) / 4 << 2;

    const size_t datalen = last / 4 * 3 + pad1 + pad2;
    unsigned char *data = new unsigned char[datalen];
    memset(data, 0, datalen);

    unsigned char *str = data;

    for (size_t i = 0; i < last; i += 4)
    {
        int n = B64index[p[i]] << 18 | B64index[p[i + 1]] << 12 | B64index[p[i + 2]] << 6 | B64index[p[i + 3]];
        str[j++] = n >> 16;
        str[j++] = n >> 8 & 0xFF;
        str[j++] = n & 0xFF;
    }
    if (pad1)
    {
        int n = B64index[p[last]] << 18 | B64index[p[last + 1]] << 12;
        str[j++] = n >> 16;
        if (pad2)
        {
            n |= B64index[p[last + 2]] << 6;
            str[j++] = n >> 8 & 0xFF;
        }
    }

    return std::unique_ptr<utl::BinaryData>(new FixedSizeBinaryData(
        std::unique_ptr<unsigned char>(data),
        datalen
    ));
}

std::unique_ptr<utl::BinaryData> utl::DefaultBase64Decoder::decode(const std::string &base64EncodedString) const
{
    return decode(base64EncodedString.c_str(), base64EncodedString.size());
}

std::string utl::DefaultBase64Decoder::decodeString(const char *base64EncodedString, const size_t len) const
{
    if (len == 0) return "";

    unsigned char *p = (unsigned char*) base64EncodedString;
    size_t j = 0,
        pad1 = len % 4 || p[len - 1] == '=',
        pad2 = pad1 && (len % 4 > 2 || p[len - 2] != '=');
    const size_t last = (len - pad1) / 4 << 2;
    std::string result(last / 4 * 3 + pad1 + pad2, '\0');
    unsigned char *str = (unsigned char*) &result[0];

    for (size_t i = 0; i < last; i += 4)
    {
        int n = B64index[p[i]] << 18 | B64index[p[i + 1]] << 12 | B64index[p[i + 2]] << 6 | B64index[p[i + 3]];
        str[j++] = n >> 16;
        str[j++] = n >> 8 & 0xFF;
        str[j++] = n & 0xFF;
    }
    if (pad1)
    {
        int n = B64index[p[last]] << 18 | B64index[p[last + 1]] << 12;
        str[j++] = n >> 16;
        if (pad2)
        {
            n |= B64index[p[last + 2]] << 6;
            str[j++] = n >> 8 & 0xFF;
        }
    }
    return result;
}

std::string utl::DefaultBase64Decoder::decodeString(const std::string &base64EncodedString) const
{
   return decodeString(base64EncodedString.c_str(), base64EncodedString.size());
}


std::unique_ptr<utl::BinaryData> utl::DefaultBase64Decoder::decodeUrlSafe(const std::string &base64EncodedString)
const
{
    return decodeUrlSafe(base64EncodedString.c_str(), base64EncodedString.size());
}


std::unique_ptr<utl::BinaryData> utl::DefaultBase64Decoder::decodeUrlSafe(
        const char *base64EncodedString,
        const size_t len
) const
{
    return decode(base64EncodedString, len);
}


std::string utl::DefaultBase64Decoder::decodeUrlSafeString(const char *base64EncodedString, const size_t numBytes) const
{
    return decodeString(base64EncodedString, numBytes);
}

std::string utl::DefaultBase64Decoder::decodeUrlSafeString(const std::string &base64EncodedString) const
{
    return decodeString(base64EncodedString);
}
