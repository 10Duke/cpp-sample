#ifndef LIBCURLHTTPCALL_H
#define LIBCURLHTTPCALL_H

#include "./httpcall.h"
#include "./httprequest.h"

#include <curl/curl.h>

namespace tenduke { namespace http { namespace curl {


/** LibCurl-based implementation of HTTPCall.
 *
 */
class LibCurlHTTPCall : public tenduke::http::HTTPCall
{
public:
    LibCurlHTTPCall(
            std::shared_ptr<const tenduke::http::HTTPRequest> request,
            bool verbose = false
    );
    virtual ~LibCurlHTTPCall();

    // tenduke::http::HTTPCall interface
    virtual std::unique_ptr<tenduke::http::HTTPResponse> execute() override;

protected:
    virtual void setRequestBody();
    virtual void setRequestHeaders();
    virtual void setRequestMethod();
    virtual void setUrl();

    const std::shared_ptr<const tenduke::http::HTTPRequest> request;
    const std::unique_ptr<CURL, decltype(curl_easy_cleanup) *> curlHandle;

    struct curl_slist *curlHeaders;
    char curlErrorMessage[CURL_ERROR_SIZE];
    bool verbose;
};


}}}

#endif // LIBCURLHTTPCALL_H
