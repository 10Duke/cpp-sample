#include "./libcurlurlparser.h"

#include "./libcurlurl.h"

namespace netcurl = tenduke::net::curl;

netcurl::LibCurlURLParser::LibCurlURLParser(bool lenient)
    : lenient(lenient)
{}


std::unique_ptr<tenduke::net::URL> netcurl::LibCurlURLParser::from(const std::string &urlToParse)
const
{
    std::unique_ptr<tenduke::net::URL> url(new LibcurlURL(lenient));

    static_cast<LibcurlURL *>(url.get())->parse(urlToParse);

    return url;
}
