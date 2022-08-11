#ifndef TENDUKE_QT_HTTP_QTHTTPCLIENT_H
#define TENDUKE_QT_HTTP_QTHTTPCLIENT_H

#include "http/httpclient.h"
#include "time/clock.h"

#include <QNetworkAccessManager>


namespace tenduke { namespace qt { namespace http {


/** QT-implementation of tenduke::http::HTTPClient.
 *
 */
class QtHTTPClient : public tenduke::http::HTTPClient
{
public:
    QtHTTPClient(
        std::shared_ptr<QNetworkAccessManager> networkAccessManager,
        long defaultTimeoutMs = tenduke::http::DEFAULT_CONNECTION_TIMEOUT_MS
    );
    QtHTTPClient(
        std::shared_ptr<QNetworkAccessManager> networkAccessManager,
        long defaultTimeoutMs,
        std::shared_ptr<tenduke::time::Clock> clock
    );

protected:
    QtHTTPClient(
        std::shared_ptr<QNetworkAccessManager> networkAccessManager,
        long defaultTimeoutMs,
        std::shared_ptr<tenduke::time::Clock> clock,
        std::shared_ptr<const tenduke::net::URLEncoder> urlEncoder
    );

    // HTTPClient interface
public:
    virtual tenduke::http::HTTPRequestBuilder request() const override;
    virtual std::unique_ptr<tenduke::http::HTTPCall> call(std::shared_ptr<const tenduke::http::HTTPRequest> request) const override;

private:
    const std::shared_ptr<QNetworkAccessManager> networkAccessManager;
    const std::shared_ptr<tenduke::time::Clock> clock;
    const std::shared_ptr<const tenduke::net::URLEncoder> urlEncoder;
    const long defaultTimeoutMs;
};


}}}

#endif // TENDUKE_QT_HTTP_QTHTTPCLIENT_H
