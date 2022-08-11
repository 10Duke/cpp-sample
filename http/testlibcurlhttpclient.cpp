#include "./testlibcurlhttpclient.h"
#include "./testhttpclient.h"
#include "./libcurlhttpclient.h"

void tenduke::http::curl::testLibCurlHTTPClient()
{
    tenduke::http::TestHTTPClient(std::unique_ptr<tenduke::http::HTTPClient>(new tenduke::http::curl::LibCurlHTTPClient(
        1000L // connection timeout of 1s
    ))).runTests();
}
