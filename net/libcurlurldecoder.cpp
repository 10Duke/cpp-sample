#include "./libcurlurldecoder.h"

#include "./urlexception.h"

#include <memory>

namespace net = tenduke::net;
namespace netcurl = tenduke::net::curl;

std::string netcurl::urlDecode(
        CURL * curl,
        const char *encodedString,
        const size_t len
) {
    int decodedLength = 0;

    std::unique_ptr<char, decltype(curl_free) *> decoded (
        curl_easy_unescape(
                curl,
                encodedString,
                len,
                &decodedLength
        ),
        curl_free
    );

    // ERROR: URL-decoding failed
    if (decoded == nullptr) {
        throw net::URLException("URL-decoding failed (libcurl)");
    }

    return std::string(decoded.get(), decodedLength);
}


std::string netcurl::LibCurlURLDecoder::decode(const char *string, const size_t len) const
{
    std::unique_ptr<CURL, decltype(curl_easy_cleanup) *> curl (curl_easy_init(), curl_easy_cleanup);

    // ERROR: curl initialization failed
    if (curl == nullptr) {
        throw net::URLException("Error: curl_easy_init() failed");
    }

    return netcurl::urlDecode(curl.get(), string, len);
}
