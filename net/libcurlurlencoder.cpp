#include "./libcurlurlencoder.h"
#include "./urlexception.h"

#include "curl/curl.h"
#include <memory>

namespace ns = tenduke::net::curl;


std::string ns::LibCurlURLEncoder::encode(const std::string &stringToEncode) const
{
    std::unique_ptr<CURL, decltype(curl_easy_cleanup) *> curl (curl_easy_init(), curl_easy_cleanup);

    // ERROR: curl initialization failed
    if (curl == nullptr) {
        throw net::URLException("Error: curl_easy_init() failed");
    }

    std::unique_ptr<char, decltype(curl_free) *> encoded (
        curl_easy_escape(
                curl.get(),
                stringToEncode.c_str(),
                stringToEncode.size()
        ),
        curl_free
    );

    // ERROR: URL-encoding failed
    if (encoded == nullptr) {
        throw net::URLException("URL-encoding failed (libcurl)");
    }

    return std::string(encoded.get());
}
