#include "./testurl.h"
#include "./libcurlurl.h"
#include "./libcurlurlparser.h"
#include "./malformedurlexception.h"

#include <iostream>

namespace net = tenduke::net;
namespace netcurl = tenduke::net::curl;

static void parseAndDump(const char * value);

void net::testURLs()
{
    parseAndDump("http://example.com?p1");
    parseAndDump("http://example.com?p1=");
    parseAndDump("http://example.com?p1=v1");
    parseAndDump("http://example.com?p1=v1&p2=v2");
    parseAndDump("http://example.com?p1=v1&p2=");
    parseAndDump("http://example.com?p1=v1&p2");

    parseAndDump("http://example.com?%C3%A4=%26");

    try {
        parseAndDump("http://example.com:-69");
    } catch (const net::MalformedURLException &e) {
        std::cerr << "! Properly detected malformed URL" << std::endl;
    }
}


static void parseAndDump(const char * value)
{
    std::cerr << "Parsing URL: " << value << std::endl;

    std::unique_ptr<net::URL> url = netcurl::LibCurlURLParser().from(value);

    for (auto const &parameter : url->getQueryParameters()) {
        std::cerr << " * " << parameter.first << "=" << parameter.second << std::endl;
    }
}
