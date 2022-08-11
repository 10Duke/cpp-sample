#include "urlbuilder.h"

namespace http = tenduke::http;
namespace net = tenduke::net;

http::URLBuilder::URLBuilder(std::shared_ptr<const net::URLEncoder> urlEncoder)
    : urlEncoder(urlEncoder)
{
}

std::string http::URLBuilder::buildString()
{
    std::string url = baseUrl;

    // build query parameters:
    bool first = true;

    for (auto const &parameter : _queryParameters) {
        if (first) {
            url += '?';
            first = false;
        }
        else {
            url += '&';
        }

        url += urlEncoder->encode(parameter.first);
        if (! parameter.second.empty()) {
            url += '=';
            url += urlEncoder->encode(parameter.second);
        }
    }

    return url;
}

http::URLBuilder & http::URLBuilder::baseURL(const std::string & url)
{
    baseUrl = url;
    return * this;
}

http::URLBuilder & http::URLBuilder::queryParameter(const std::string &name)
{
    return queryParameter(name, "");
}

http::URLBuilder & http::URLBuilder::queryParameter(
        const std::string &name,
        const std::string &value
)
{
    _queryParameters.emplace(name, value);
    return *this;
}

http::URLBuilder & http::URLBuilder::queryParameters(const std::map<std::string, std::string> &parameters)
{
    for (auto const &parameter : parameters) {
        _queryParameters.emplace(parameter.first, parameter.second);
    }
    return *this;
}


