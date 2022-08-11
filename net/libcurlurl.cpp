#include "./libcurlurl.h"
#include "./libcurlurldecoder.h"
#include "./malformedurlexception.h"

#include <cstring>
#include <memory>
#include <new>

namespace net = tenduke::net;
namespace netcurl = tenduke::net::curl;

netcurl::LibcurlURL::LibcurlURL(bool lenient)
    : lenient(lenient)
{
    curlHandle = curl_url();
    if (curlHandle == nullptr) {
        throw std::bad_alloc();
    }
}

netcurl::LibcurlURL::~LibcurlURL()
{
    curl_url_cleanup(curlHandle);
}

void netcurl::LibcurlURL::parse(const std::string &url)
{
    unsigned int flags = 0;

    if (lenient) {
        flags |= CURLU_NON_SUPPORT_SCHEME;
    }

    CURLUcode status = curl_url_set(
        curlHandle,
        CURLUPART_URL,
        url.c_str(),
        flags
    );

    switch(status)
    {
        case CURLUE_OK:
            break;
        case CURLUE_OUT_OF_MEMORY:
            throw std::bad_alloc();
        default:
            throw net::MalformedURLException("Invalid URL (libcurl = " + std::to_string(status) + ')');
    }
}

std::multimap<std::string, std::string> netcurl::LibcurlURL::getQueryParameters()
const
{
    char * queryStringPtr;

    // Extract the query string, do not URL-decode
    CURLUcode status = curl_url_get(
        curlHandle,
        CURLUPART_QUERY,
        &queryStringPtr,
        0
    );

    switch(status)
    {
        case CURLUE_OK:
            break;
        case CURLUE_OUT_OF_MEMORY:
            throw std::bad_alloc();
        default:
            throw net::URLException("Extracting query failed (libcurl = " + std::to_string(status) + ')');
    }

    std::unique_ptr<char, decltype(curl_free) *> queryString (queryStringPtr, curl_free);

    // EXIT: No query
    if (queryString == nullptr) {
        return std::multimap<std::string, std::string>();
    }

    return parseQueryString(queryString.get());
}

std::multimap<std::string, std::string> netcurl::parseQueryString(const char * const queryString)
{
    // Initialize Curl-handle:
    std::unique_ptr<CURL, decltype(curl_easy_cleanup) *> curl (curl_easy_init(), curl_easy_cleanup);

    // ERROR: Curl initialization failed
    if (curl == nullptr) {
        throw net::URLException("libcurl initialization failed");
    }

    int queryStringLen = std::strlen(queryString);
    int ampersandIdx = -1;
    const char * parameter = queryString;
    std::multimap<std::string, std::string> queryParameters;

    while (parameter != nullptr) {
        const char * ampersandPtr = std::strchr(parameter, '&');
        int parameterLen;

        // 0123456
        //         len=0
        //                   plen=0
        // a=b&c=d len=7
        //            nai=3  plen=3 ai=3
        //                   plen=3
        // a=b     len=3  3 --1 - 1 = 3

        // No & found anymore:
        if (ampersandPtr == nullptr) {
            parameterLen = queryStringLen - ampersandIdx - 1;
        }
        // Found &
        else {
            int newAmpersandIdx = ampersandPtr - parameter;
            parameterLen = newAmpersandIdx - ampersandIdx - 1;
            ampersandIdx = newAmpersandIdx;
            ampersandPtr++;
        }

        // Parse parameter:
        if (parameterLen > 0) {
            const char * ptr = parameter;
            int equalSign = -1;

            for (int i = 0; i < parameterLen; i++) {
                if (*ptr++ == '=') {
                    equalSign = i;
                    break;
                }
            }
            // No parameter name (e.g. "=true"):
            if (equalSign == 0) {
                // Skip.
            }
            // No value (e.g. "name")
            else if (equalSign < 0) {
                queryParameters.emplace(netcurl::urlDecode(curl.get(), parameter, parameterLen), "");
            }
            // No value (e.g. "name=")
            else if (equalSign == parameterLen - 1) {
                queryParameters.emplace(netcurl::urlDecode(curl.get(), parameter, parameterLen -1), "");
            }
            // Parameter has value (e.g. "name=value"):
            else {
                // 0123456789
                // name=value
                //   len = 10
                //   equalSign = 4
                queryParameters.emplace(
                    netcurl::urlDecode(curl.get(), parameter, equalSign),
                    netcurl::urlDecode(curl.get(), parameter + equalSign + 1, parameterLen - equalSign - 1)
                );
            }
        }

        parameter = ampersandPtr;
    }

    return queryParameters;

}
