#ifndef TENDUKE_HTTP_TESTHTTPCLIENT_H
#define TENDUKE_HTTP_TESTHTTPCLIENT_H


#include "./httpclient.h"

#include <memory>

namespace tenduke { namespace http {

class TestHTTPClient
{
public:
    TestHTTPClient(std::unique_ptr<tenduke::http::HTTPClient> httpClient)
     : http(std::move(httpClient))
    {}

    void runTests();

    void expectHTTP200();
    void expectHTTP400();
    void expectHTTP401();
    void expectHTTP403();
    void expectHTTP404();
    void expectHTTP415();
    void expectUnknownHostException();
    void expectRequestTimedOutException();

protected:
    const std::unique_ptr<tenduke::http::HTTPClient> http;
};

}}

#endif // TENDUKE_HTTP_TESTHTTPCLIENT_H
