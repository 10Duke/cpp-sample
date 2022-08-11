#ifndef URLBUILDER_H
#define URLBUILDER_H

#include "net/urlencoder.h"

#include <map>
#include <memory>
#include <string>

namespace tenduke { namespace http {

/** Very simple URL-builder.
 *
 *  Only part of URL-construction implemented.
 */
class URLBuilder
{
public:
    URLBuilder(std::shared_ptr<const tenduke::net::URLEncoder> urlEncoder);

    std::string buildString();

    URLBuilder & baseURL(const std::string &url);
    URLBuilder & queryParameter(const std::string &name);
    URLBuilder & queryParameter(const std::string &name, const std::string &value);
    URLBuilder & queryParameters(const std::map<std::string, std::string> &parameters);

private:
    const std::shared_ptr<const tenduke::net::URLEncoder> urlEncoder;

    std::string baseUrl;
    std::multimap<std::string, std::string> _queryParameters;
};

}}

#endif // URLBUILDER_H
