#ifndef HTTPREQUESTBUILDER_H
#define HTTPREQUESTBUILDER_H

#include "./httprequest.h"
#include "./urlbuilder.h"
#include "net/urlencoder.h"

#include <string>
#include <memory>
#include <map>

namespace tenduke { namespace http {


/** Builds HTTPRequest.
 *
 */
class HTTPRequestBuilder
{
public:
    HTTPRequestBuilder(
            std::shared_ptr<const tenduke::net::URLEncoder> urlEncoder,
            long connectionTimeoutMs = tenduke::http::DEFAULT_CONNECTION_TIMEOUT_MS,
            long timeoutMs = tenduke::http::DEFAULT_TIMEOUT_MS
    );

    std::unique_ptr<HTTPRequest> build();

    HTTPRequestBuilder & get();
    HTTPRequestBuilder & post();
    HTTPRequestBuilder & url(const std::string &url);
    HTTPRequestBuilder & header(const std::string &name, const std::string &value);
    HTTPRequestBuilder & formParameter(const std::string &name, const std::string &value);
    HTTPRequestBuilder & contentType(const std::string &contentType);
    HTTPRequestBuilder & charset(const std::string &charset);
    HTTPRequestBuilder & utf8();

    URLBuilder url();


protected:
    std::string buildMessageBodyFromFormParameters();

private:
    const std::shared_ptr<const tenduke::net::URLEncoder> urlEncoder;

    std::string method;
    std::string URL;
    std::multimap<std::string, std::string> headers;
    std::multimap<std::string, std::string> formParameters;
    std::string requestContentType;
    std::string requestCharset;

    long connectionTimeoutMs;
    long timeoutMs;
};


}}

#endif // HTTPREQUESTBUILDER_H
