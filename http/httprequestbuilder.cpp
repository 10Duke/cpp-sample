#include "./httprequestbuilder.h"

namespace http = tenduke::http;
namespace net = tenduke::net;

http::HTTPRequestBuilder::HTTPRequestBuilder(
        std::shared_ptr<const net::URLEncoder> urlEncoder,
        long connectionTimeoutMs,
        long timeoutMs
)
    : urlEncoder(urlEncoder), method("GET"), connectionTimeoutMs(connectionTimeoutMs), timeoutMs(timeoutMs)
{
}

std::unique_ptr<http::HTTPRequest> http::HTTPRequestBuilder::build()
{
    std::string messageBody;

    if (! formParameters.empty()) {
        messageBody = buildMessageBodyFromFormParameters();
    }

    if (! requestContentType.empty()) {
        std::string contentType = requestContentType;
        if (!requestCharset.empty()) {
            contentType += "; charset=";
            contentType += requestCharset;
        }
        headers.emplace("Content-Type", contentType);
    }

    return std::unique_ptr<http::HTTPRequest>(new http::HTTPRequest(
        method,
        URL,
        headers,
        messageBody,
        connectionTimeoutMs,
        timeoutMs
    ));
}

std::string http::HTTPRequestBuilder::buildMessageBodyFromFormParameters()
{
    std::string messageBody;
    bool first = true;

    for (auto const & formParameter : formParameters) {
        if (first) {
            first = false;
        } else {
            messageBody += '&';
        }
        messageBody += urlEncoder->encode(formParameter.first);
        messageBody += '=';
        messageBody += urlEncoder->encode(formParameter.second);
    }

    return messageBody;
}

http::HTTPRequestBuilder & http::HTTPRequestBuilder::get()
{
    method = "GET";
    return *this;
}

http::HTTPRequestBuilder & http::HTTPRequestBuilder::post()
{
    method = "POST";
    return *this;
}

http::HTTPRequestBuilder & http::HTTPRequestBuilder::url(const std::string &url)
{
    this->URL = url;
    return *this;
}

http::HTTPRequestBuilder & http::HTTPRequestBuilder::formParameter(
        const std::string &name,
        const std::string &value
) {
    formParameters.emplace(name, value);
    return *this;
}

http::HTTPRequestBuilder & http::HTTPRequestBuilder::header(const std::string &name, const std::string &value)
{
    headers.emplace(name, value);
    return *this;
}

http::HTTPRequestBuilder & http::HTTPRequestBuilder::contentType(const std::string &contentType)
{
    requestContentType = contentType;
    return *this;
}

http::HTTPRequestBuilder & http::HTTPRequestBuilder::charset(const std::string &charset)
{
    requestCharset = charset;
    return *this;
}

http::HTTPRequestBuilder & http::HTTPRequestBuilder::utf8()
{
    return charset("UTF-8");
}

http::URLBuilder http::HTTPRequestBuilder::url()
{
    return http::URLBuilder(urlEncoder);
}
