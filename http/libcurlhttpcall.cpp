#include "./libcurlhttpcall.h"
#include "./httprequestexception.h"

#include "net/networkingexception.h"
#include "net/malformedurlexception.h"
#include "net/requesttimedout.h"
#include "net/unknownhostexception.h"
#include "utl/simplebuffer.h"

#include <cmath>

namespace http = tenduke::http;
namespace curl = tenduke::http::curl;
namespace net = tenduke::net;

/** Checks if given character is an OWS-character as defined in Section 3.2.3. of RFC 7230.
 *
 *  @param c the character
 *  @return true if an OWS character, false otherwise
 */
static bool isOWS(char c)
{
    return (c == ' ' || c == '\t');
}


/** Checks if given character is NOT an OWS-character as defined in Section 3.2.3. of RFC 7230.
 *
 *  @param c the character
 *  @return true if not an OWS character, false otherwise
 */
static bool isNotOWS(char c)
{
    return !isOWS(c);
}


/** LibCurl callback function for reading response payload.
 *
 */
static size_t readCurlResponsePayload(
    void *data,
    size_t size,
    size_t nmemb,
    void *userp
) {
    const size_t numBytes = size * nmemb;

    tenduke::utl::SimpleBuffer * buffer = static_cast<tenduke::utl::SimpleBuffer *>(userp);

    buffer->append((unsigned char*)data, numBytes);

    return numBytes;
}


/** LibCurl callback function for reading response header line.
 *
 */
static size_t responseHeaderReceived(
        char *buffer,
        size_t size,
        size_t nitems,
        void *userdata
) {
    char *ptr = buffer;
    int len = nitems;
    int idxColon = -1; // Index of colon, -1 if no colon in the line

    // find the colon (manual parsing FTW!):
    while(idxColon < len) {
        idxColon++;
        if (*ptr == ':') {
           break;
        }
        ptr++;
    }

    // colon found and is not leading:
    if (idxColon > 0) {
        // Find start of the value, ignore whitespace:
        int idxValue = idxColon - 1;
        while (idxValue < len) {
            idxValue++;
            if (isNotOWS(*ptr)) {
                break;
            }
        }
        // there is a value:
        if (idxValue < len) {
            // Find end of the value, ignore trailing whitespace:
            int idxValueEnd = len - 1;
            while (idxValueEnd > idxValue) {
                if (isNotOWS(buffer[idxValueEnd])) {
                    break;
                }
                idxValueEnd--;
            }

            // We have a header, yay
            std::multimap<std::string, std::string> *responseHeaders = static_cast<std::multimap<std::string, std::string> *>(userdata);
            responseHeaders->emplace(
                        std::string(buffer, idxColon - 1),
                        std::string(buffer + idxValue, idxValueEnd - idxValue + 1)
            );
        }
    }

    return nitems * size;
}


curl::LibCurlHTTPCall::LibCurlHTTPCall(
        std::shared_ptr<const http::HTTPRequest> request,
        bool verbose
)
    : request(std::move(request)), curlHandle(curl_easy_init(), curl_easy_cleanup), curlHeaders(nullptr), verbose(verbose)
{
    if (curlHandle == nullptr) {
        throw http::HTTPRequestException("libcurl initialization failed");
    }

    curlErrorMessage[0] = '\0';
}


curl::LibCurlHTTPCall::~LibCurlHTTPCall()
{
    if (curlHeaders != nullptr) {
        curl_slist_free_all(curlHeaders);
    }
}


static std::string getEffectiveURL(CURL *curlHandle)
{
    char * url = nullptr;

    CURLcode status = curl_easy_getinfo(curlHandle, CURLINFO_EFFECTIVE_URL, &url);
    if (status != CURLE_OK) {
        // Ignored intentionally. Used in error reporting anyway.
    }

    if (url == nullptr) {
        return std::string("");
    }

    return std::string(url);
}


static std::string getEffectiveHostName(CURL * curlHandle)
{
    CURLU *urlHandle = curl_url();

    CURLUcode rc = curl_url_set(
                urlHandle,
                CURLUPART_URL,
                getEffectiveURL(curlHandle).c_str(),
                0
    );
    if (rc != CURLUE_OK) {
        // Ignored intentionally. This method is used for error reporting anyway
        // Also, the effective URL should be valid.
    }

    char * hostName = nullptr;
    rc = curl_url_get(urlHandle, CURLUPART_HOST, &hostName, 0);
    if (rc != CURLUE_OK) {
        // Ignored intentionally
    }

    std::string hn = std::string(hostName);

    curl_free(hostName);
    curl_url_cleanup(urlHandle);

    return hn;
}


std::unique_ptr<http::HTTPResponse> curl::LibCurlHTTPCall::execute()
{
    setUrl();
    setRequestHeaders();
    if (request->hasBody()) {
        setRequestBody();
    }
    setRequestMethod();

    // Set response body consumer
    // This is a very simple implementation.
    std::unique_ptr<tenduke::utl::BinaryData> responseBody(new tenduke::utl::SimpleBuffer());

    curl_easy_setopt(curlHandle.get(), CURLOPT_WRITEFUNCTION, readCurlResponsePayload); // always returns CURLE_OK
    curl_easy_setopt(curlHandle.get(), CURLOPT_WRITEDATA, responseBody.get()); // always returns CURLE_OK

    // Set response header consumer
    std::multimap<std::string, std::string> responseHeaders;
    curl_easy_setopt(curlHandle.get(), CURLOPT_HEADERFUNCTION, responseHeaderReceived); // always returns CURLE_OK
    curl_easy_setopt(curlHandle.get(), CURLOPT_HEADERDATA, &responseHeaders); // always returns CURLE_OK

    if (verbose) {
        curl_easy_setopt(curlHandle.get(), CURLOPT_VERBOSE, 1L); //always returns CURLE_OK
    }
    curl_easy_setopt(curlHandle.get(), CURLOPT_CONNECTTIMEOUT_MS, request->getConnectionTimeoutMs()); // always returns CURLE_OK
    curl_easy_setopt(curlHandle.get(), CURLOPT_TIMEOUT, std::roundl(((double)request->getTimeoutMs()) / 1000L)); // always returns CURLE_OK

    CURLcode status = curl_easy_perform(curlHandle.get());
    switch(status) {
        case CURLE_OK:
            break;
        case CURLE_COULDNT_RESOLVE_HOST:
            throw net::UnknownHostException("Unknown host: " + getEffectiveHostName(curlHandle.get()));
        case CURLE_COULDNT_CONNECT:
            throw net::NetworkingException("Unable to connect to: " + getEffectiveHostName(curlHandle.get()));
        case CURLE_URL_MALFORMAT:
            throw net::MalformedURLException("Malformed URL: " + getEffectiveURL(curlHandle.get()));
        case CURLE_OPERATION_TIMEDOUT:
            throw net::RequestTimedOut("Connection to host timed out: " + getEffectiveHostName(curlHandle.get()));
        case CURLE_SEND_ERROR:
            throw net::NetworkingException("I/O error sending data");
        case CURLE_RECV_ERROR:
            throw net::NetworkingException("I/O error receiving data");
        default:
            throw net::NetworkingException("Curl call failed");
    }

    long httpStatusCode;
    status = curl_easy_getinfo(curlHandle.get(), CURLINFO_RESPONSE_CODE, &httpStatusCode);
    if (status != CURLE_OK) {
        // ignored intentionally
    }

    return std::unique_ptr<http::HTTPResponse>(new http::HTTPResponse(
        (int)httpStatusCode,
        responseHeaders,
        std::move(responseBody)
    ));
}


void curl::LibCurlHTTPCall::setUrl()
{
    CURLcode status = curl_easy_setopt(curlHandle.get(), CURLOPT_URL, request->getUrl().c_str());
    if (status != CURLE_OK) {
        throw http::HTTPRequestException("Curl rejected the URL: " + request->getUrl());
    }
}


void curl::LibCurlHTTPCall::setRequestMethod()
{
    CURLcode status = curl_easy_setopt(curlHandle.get(), CURLOPT_CUSTOMREQUEST, request->getMethod().c_str());
    if (status != CURLE_OK) {
        throw http::HTTPRequestException("Curl rejected the HTTP method \"" + request->getMethod() + "\"");
    }
}


void curl::LibCurlHTTPCall::setRequestBody()
{
    const std::string & data = request->getMessageBody();

    curl_easy_setopt(curlHandle.get(), CURLOPT_POSTFIELDS, data.c_str());
    CURLcode status = curl_easy_setopt(curlHandle.get(), CURLOPT_POSTFIELDSIZE, (long) data.size());
    if (status != CURLE_OK) {
        throw http::HTTPRequestException("Curl rejected request body");
    }
}

void curl::LibCurlHTTPCall::setRequestHeaders()
{
    if (request->getHeaders().empty()) {
        // No headers: Nothing to do
        return;
    }

    for (auto entry : request->getHeaders()) {
        std::string value = entry.first + ": " + entry.second;

        curlHeaders = curl_slist_append(curlHeaders, value.c_str());
    }

    CURLcode status = curl_easy_setopt(curlHandle.get(), CURLOPT_HTTPHEADER, curlHeaders);
    if (status != CURLE_OK) {
        throw http::HTTPRequestException("Curl rejected request headers");
    }
}

