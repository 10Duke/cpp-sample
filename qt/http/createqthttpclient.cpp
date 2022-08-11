#include "./createqthttpclient.h"
#include "http/httpclient.h"
#include "qt/http/qthttpclient.h"
#include "time/defaultclock.h"

namespace qthttp = tenduke::qt::http;
namespace xdhttp = tenduke::http;
namespace xdtime = tenduke::time;

std::unique_ptr<xdhttp::HTTPClient> qthttp::createQtHTTPClient(
    std::shared_ptr<QNetworkAccessManager> networkAccessManager
)
{
    return qthttp::createQtHTTPClient(
        networkAccessManager,
        std::shared_ptr<xdtime::Clock>(new xdtime::DefaultClock)
    );
}

std::unique_ptr<xdhttp::HTTPClient> qthttp::createQtHTTPClient(
    std::shared_ptr<QNetworkAccessManager> networkAccessManager,
    std::shared_ptr<xdtime::Clock> clock
)
{
    return std::unique_ptr<xdhttp::HTTPClient>(new qthttp::QtHTTPClient(
        networkAccessManager,
        xdhttp::DEFAULT_CONNECTION_TIMEOUT_MS,
        clock
    ));
}

