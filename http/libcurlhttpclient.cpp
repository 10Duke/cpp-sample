#include "./libcurlhttpclient.h"
#include "./libcurlhttpcall.h"
#include "net/libcurlurlencoder.h"


namespace http = tenduke::http;
namespace net = tenduke::net;
namespace netcurl = tenduke::net::curl;
namespace xdcurl = tenduke::http::curl;

xdcurl::LibCurlHTTPClient::LibCurlHTTPClient(
        long defaultConnectionTimeoutMs,
        long defaultTimeoutMs,
        bool verbose
)
    : xdcurl::LibCurlHTTPClient(
          std::shared_ptr<net::URLEncoder>(new netcurl::LibCurlURLEncoder()),
          defaultConnectionTimeoutMs,
          defaultTimeoutMs,
          verbose
      )
{}

xdcurl::LibCurlHTTPClient::LibCurlHTTPClient(
    std::shared_ptr<const net::URLEncoder> urlEncoder,
    long defaultConnectionTimeoutMs,
    long defaultTimeoutMs,
    bool verbose
)
    : urlEncoder(urlEncoder), defaultConnectionTimeoutMs(defaultConnectionTimeoutMs), defaultTimeoutMs(defaultTimeoutMs), verbose(verbose)
{}

xdcurl::LibCurlHTTPClient::~LibCurlHTTPClient()
{}

http::HTTPRequestBuilder xdcurl::LibCurlHTTPClient::request() const
{
    return http::HTTPRequestBuilder(urlEncoder, defaultConnectionTimeoutMs, defaultTimeoutMs);
}

std::unique_ptr<http::HTTPCall> xdcurl::LibCurlHTTPClient::call(std::shared_ptr<const http::HTTPRequest> request) const
{
    return std::unique_ptr<http::HTTPCall>(new xdcurl::LibCurlHTTPCall(request, verbose));
}
