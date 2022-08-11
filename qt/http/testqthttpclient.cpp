#include "./testqthttpclient.h"

#include "./qthttpclient.h"
#include "http/testhttpclient.h"

namespace xdhttp = tenduke::http;
namespace xdtime = tenduke::time;

void tenduke::qt::http::testQtHttpClient()
{
    std::shared_ptr<QNetworkAccessManager> networkAccessManager(new QNetworkAccessManager());

    xdhttp::TestHTTPClient(std::unique_ptr<xdhttp::HTTPClient>(new tenduke::qt::http::QtHTTPClient(
        networkAccessManager,
        1000L // timeout of 1s
    ))).runTests();
}


