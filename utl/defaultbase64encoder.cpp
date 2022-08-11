#include "defaultbase64encoder.h"

namespace utl = tenduke::utl;

// Implementation from:
// https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c/37109258#37109258
// https://stackoverflow.com/questions/342409/how-do-i-base64-encode-decode-in-c

static const char* B64chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string utl::DefaultBase64Encoder::encode(
        const unsigned char *data,
        const size_t len
) const
{
    std::string result((len + 2) / 3 * 4, '=');
    unsigned char *p = (unsigned  char*) data;
    char *str = &result[0];
    size_t j = 0, pad = len % 3;
    const size_t last = len - pad;

    for (size_t i = 0; i < last; i += 3)
    {
        int n = int(p[i]) << 16 | int(p[i + 1]) << 8 | p[i + 2];
        str[j++] = B64chars[n >> 18];
        str[j++] = B64chars[n >> 12 & 0x3F];
        str[j++] = B64chars[n >> 6 & 0x3F];
        str[j++] = B64chars[n & 0x3F];
    }
    if (pad)  /// Set padding
    {
        int n = --pad ? int(p[last]) << 8 | p[last + 1] : p[last];
        str[j++] = B64chars[pad ? n >> 10 & 0x3F : n >> 2];
        str[j++] = B64chars[pad ? n >> 4 & 0x03F : n << 4 & 0x3F];
        str[j++] = pad ? B64chars[n << 2 & 0x3F] : '=';
    }
    return result;
}

std::string utl::DefaultBase64Encoder::encode(const std::string &string) const
{
    return encode((const unsigned char *)string.c_str(), string.size());
}

std::string utl::DefaultBase64Encoder::encodeUrlSafe(
        const unsigned char *data,
        const size_t numBytes
) const
{
    // This is a q'n'd dumb implementation
    // TODO: Encode directly using URLSafe-alphabet and skipping padding
    std::string encoded = encode(data, numBytes);
    int i = encoded.size() - 1;

    // Erase padding (trailing '=')
    while(i >= 0 && encoded[i] == '=') {
        encoded.erase(i, 1);
        i--;
    }

    // Change the chars to URL-safe:
    int len = encoded.size();
    for (int i = 0; i < len; i++) {
        if (encoded[i] == '+') {
            encoded[i] = '-';
        } else if (encoded[i] == '/') {
            encoded[i] = '_';
        }
    }

    return encoded;
}

std::string utl::DefaultBase64Encoder::encodeUrlSafe(const std::string &string) const
{
    return encodeUrlSafe((const unsigned char *)string.c_str(), string.size());
}
