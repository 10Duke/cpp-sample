#include "./qthttpclient.h"
#include "./qthttpcall.h"
#include "qt/net/qturlencoder.h"
#include "time/defaultclock.h"

namespace qthttp = tenduke::qt::http;
namespace qtnet = tenduke::qt::net;
namespace xdtime = tenduke::time;
namespace xdhttp = tenduke::http;
namespace xdnet = tenduke::net;

qthttp::QtHTTPClient::QtHTTPClient(
    std::shared_ptr<QNetworkAccessManager> networkAccessManager,
    long defaultTimeoutMs
)
    : QtHTTPClient(networkAccessManager, defaultTimeoutMs, std::shared_ptr<xdtime::Clock>(new xdtime::DefaultClock()))
{
}

qthttp::QtHTTPClient::QtHTTPClient(
    std::shared_ptr<QNetworkAccessManager> networkAccessManager,
    long defaultTimeoutMs,
    std::shared_ptr<tenduke::time::Clock> clock
)
 : QtHTTPClient(networkAccessManager, defaultTimeoutMs, clock, std::shared_ptr<const xdnet::URLEncoder>(new qtnet::QtURLEncoder()))
{
}


qthttp::QtHTTPClient::QtHTTPClient(
    std::shared_ptr<QNetworkAccessManager> networkAccessManager,
    long defaultTimeoutMs,
    std::shared_ptr<tenduke::time::Clock> clock,
    std::shared_ptr<const tenduke::net::URLEncoder> urlEncoder
) : networkAccessManager(networkAccessManager), clock(clock), urlEncoder(urlEncoder), defaultTimeoutMs(defaultTimeoutMs)
{
}


xdhttp::HTTPRequestBuilder qthttp::QtHTTPClient::request() const
{
    return xdhttp::HTTPRequestBuilder(
                urlEncoder,
                xdhttp::DEFAULT_CONNECTION_TIMEOUT_MS,
                defaultTimeoutMs
    );
}


std::unique_ptr<xdhttp::HTTPCall> qthttp::QtHTTPClient::call(std::shared_ptr<const xdhttp::HTTPRequest> request) const
{
    return std::unique_ptr<xdhttp::HTTPCall>(new qthttp::QtHTTPCall(
        request,
        networkAccessManager,
        clock
    ));
}
