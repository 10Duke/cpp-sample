#ifndef LIBCURLHTTPCLIENT_H
#define LIBCURLHTTPCLIENT_H

#include "./httpclient.h"
#include "net/urlencoder.h"

namespace tenduke { namespace http { namespace curl {


/** LibCurl -based implementation of HTTPClient. Very simple currently, does not support all variations.
 *
 */
class LibCurlHTTPClient : public tenduke::http::HTTPClient
{
public:
    /** Constructs instance with LibCurlURLEncoder. */
    LibCurlHTTPClient(
            long defaultConnectionTimeoutMs = DEFAULT_CONNECTION_TIMEOUT_MS,
            long defaultTimeoutMs = DEFAULT_TIMEOUT_MS,
            bool verbose = false
    );

    /** Constructs instance with given URLEncoder. */
    LibCurlHTTPClient(
            std::shared_ptr<const tenduke::net::URLEncoder> urlEncoder,
            long defaultConnectionTimeoutMs = DEFAULT_CONNECTION_TIMEOUT_MS,
            long defaultTimeoutMs = DEFAULT_TIMEOUT_MS,
            bool verbose = false
    );

    virtual ~LibCurlHTTPClient();

    // tenduke::http::HTTPClient interface
    virtual tenduke::http::HTTPRequestBuilder request() const override;
    virtual std::unique_ptr<tenduke::http::HTTPCall> call(std::shared_ptr<const tenduke::http::HTTPRequest> request) const override;

private:
    const std::shared_ptr<const tenduke::net::URLEncoder> urlEncoder;
    const long defaultConnectionTimeoutMs;
    const long defaultTimeoutMs;
    const bool verbose;
};


}}}

#endif // LIBCURLHTTPCLIENT_H
