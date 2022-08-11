#include "testhttpclient.h"

#include "net/requesttimedout.h"
#include "net/unknownhostexception.h"

#include <iostream>

namespace xdhttp = tenduke::http;


void xdhttp::TestHTTPClient::runTests()
{
    expectHTTP200();
    expectHTTP400();
    expectHTTP401();
    expectHTTP403();
    expectHTTP404();
    expectHTTP415();
    expectUnknownHostException();
    expectRequestTimedOutException();
}


void xdhttp::TestHTTPClient::expectHTTP200()
{
    std::unique_ptr<xdhttp::HTTPResponse> response = http->call(
                http->request().get().url("https://genco.10duke.com/user/")
                .build()
    )->execute();

    if (response->getStatusCode() != 200) {
        throw std::runtime_error("Expected HTTP status code 200, got " + std::to_string(response->getStatusCode()));
    }
}


void xdhttp::TestHTTPClient::expectHTTP400()
{
    std::unique_ptr<xdhttp::HTTPResponse> response = http->call(
                http->request().post().url("https://genco.10duke.com/user/oauth20/token")
                .formParameter("grant_type", "no-such-type")
                .contentType("application/x-www-form-urlencoded")
                .charset("UTF-8")
                .build()
    )->execute();

    if (response->getStatusCode() != 400) {
        throw std::runtime_error("Expected HTTP status code 400, got " + std::to_string(response->getStatusCode()));
    }
}


void xdhttp::TestHTTPClient::expectHTTP401()
{
    std::unique_ptr<xdhttp::HTTPResponse> response = http->call(
                http->request().get().url("https://genco.10duke.com/user/api/0.exp/sessions/authentications").build()
    )->execute();

    if (response->getStatusCode() != 401) {
        throw std::runtime_error("Expected HTTP status code 401, got " + std::to_string(response->getStatusCode()));
    }
}

void xdhttp::TestHTTPClient::expectHTTP403()
{
    std::unique_ptr<xdhttp::HTTPResponse> response = http->call(
                http->request().get().url("https://genco.10duke.com/api/idp/v1.exp/users").build()
    )->execute();

    if (response->getStatusCode() != 403) {
        throw std::runtime_error("Expected HTTP status code 403, got " + std::to_string(response->getStatusCode()));
    }
}


void xdhttp::TestHTTPClient::expectHTTP404()
{
    std::unique_ptr<xdhttp::HTTPResponse> response = http->call(
                http->request().get().url("https://genco.10duke.com/api/idp/v1.exp/userz").build()
    )->execute();

    if (response->getStatusCode() != 404) {
        throw std::runtime_error("Expected HTTP status code 404, got " + std::to_string(response->getStatusCode()));
    }
}


/** HTTP 415 invalid content type. */
void xdhttp::TestHTTPClient::expectHTTP415()
{
    std::unique_ptr<xdhttp::HTTPResponse> response = http->call(
                http->request().post().url("https://genco.10duke.com/api/idp/v1.exp/users")
                .contentType("application/x-www-form-urlencoded")
                .build()
    )->execute();

    if (response->getStatusCode() != 415) {
        throw std::runtime_error("Expected HTTP status code 415, got " + std::to_string(response->getStatusCode()));
    }
}


void xdhttp::TestHTTPClient::expectUnknownHostException()
{
    try {
        http->call(http->request().get().url("https://no-such-subdomain.10duke.com").build())->execute();
    }
    catch (const tenduke::net::UnknownHostException &e) {
        return;
    }

    throw std::runtime_error("Should have thrown UnknownHostException");
}


void xdhttp::TestHTTPClient::expectRequestTimedOutException()
{
    try {
        http->call(http->request().get().url("http://example.com:12345").build())->execute();
    }
    catch (const tenduke::net::RequestTimedOut &e) {
        std::cerr <<e.what() << std::endl;
        return;
    }

    throw std::runtime_error("Should have thrown RequestTimedOut");
}

