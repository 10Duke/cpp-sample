#include "./testdefaultlicensing2.h"

#include "crypto/rsapublickeyfrompemstring.h"
#include "http/libcurlhttpclient.h"
#include "json/cjsonparser.h"
#include "licensing/defaultlicensingclient.h"
#include "licensing/licensecheckoutparametersbuilder.h"
#include "oauth/oauthconfiguration.h"
#include "oidc/createdefaultoidcclient.h"
#include "oidc/oidcconfiguration.h"
#include "oidc/oidcclient.h"
#include "time/defaultclock.h"
#include "utl/random/insecurerandombytes.h"

#include <iostream>
#include <string>

namespace xdcjson = tenduke::json::cjson;
namespace libcrypto = tenduke::crypto::libcrypto;
namespace xdcrypto = tenduke::crypto;
namespace xdcurlhttp = tenduke::http::curl;
namespace xdhttp = tenduke::http;
namespace xdjson = tenduke::json;
namespace xdlicensing = tenduke::licensing;
namespace xdoauth = tenduke::oauth;
namespace xdoidc = tenduke::oauth::oidc;
namespace xdrandom = tenduke::utl::random;
namespace xdtime = tenduke::time;

static std::string ID_TOKEN_VERIFICATION_KEY_RSA_PEM =
        "-----BEGIN PUBLIC KEY-----\n"
        "MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEAywVSSuHKmyNrcT8JArxo\n"
        "IqTuWdCvG2R78p1Osdav8ivjQWqDnjR37tt7L+U+sopV4ka4gUQVi7Ie87l2cJwh\n"
        "sJ6uAQWfp6K7r+H/yH+ak+F8EvcWLFNqRjbvgAu0MqSt16bkZX01AanBca3yioZ+\n"
        "Ihe7DryKSbR1n8IMU7DRUiZzB4c9qdPphmDwxzryaiTkE1QJyXGjpSdvwwIdXE9u\n"
        "XE12zSeR2+CRKWTPZsnRBKpSDdrEwE8nSRW5XmDppnpoAvb6YI7ULtXZN354atbH\n"
        "sC1s+siHsjD7zB//cUzsRtge4YCTOoIs4thirizP3uXg8xJSs1Quie1GvZt0ufwl\n"
        "jMQnbBR7Le1ctV7sCZFom4XJGewGpnXQP9TBBpofH1RhjmBBRyruvbX3xGj2mKpi\n"
        "hy6k3FzoxZ580Pv1KGo1CYjLgfXFSmwnq/MJ6bE1wR9rmexOE1b2laWsTbTdpZB4\n"
        "/3mHGQ1yd5w+7ZjOQ1/K0g5FHm5yKK9cJSvQihN/BpGN5YhvwkpjhAhJlF+csLg4\n"
        "DGXl5GxnTfP1ZSUywOP2Da4PzpaghsDJpkBkh6rKDK+mJ9v0He1BfvhxIqAjVnur\n"
        "IRZriZ6mXwTM7C9v30IBIgnadgLkyptuj+/1F3Z3m2+Ix6uLpZGUQpWVgMcC2uM5\n"
        "kcU6rWjrfEfAVM23axgy7c0CAwEAAQ==\n"
        "-----END PUBLIC KEY-----"
;

static std::unique_ptr<const xdcrypto::PublicKey> createPublicKey(std::string key)
{
    if (key.empty()) {
        return std::unique_ptr<const xdcrypto::PublicKey>();
    }

    // NOTE: This will throw CryptoException if the key is invalid.
    return libcrypto::RSAPublicKeyFromPEMString().from(key);
}


void tenduke::tst::licensing::testDefaultLicensing()
{
    // Configuration:
    std::shared_ptr<xdoauth::OAuthConfiguration> oauthConfig (new xdoauth::OAuthConfiguration(
        "https://genco.10duke.com/user/oauth20/authz",
        "https://genco.10duke.com/user/oauth20/token",
        "QTDemo",
        "FQdL!XSYYrQ7-zeiCe",
        "tenduke://callback",
        true
    ));
    std::shared_ptr<xdoidc::OIDCConfiguration> oidcConfig (new xdoidc::OIDCConfiguration(
        "https://genco.10duke.com",
        createPublicKey(ID_TOKEN_VERIFICATION_KEY_RSA_PEM),
        "https://genco.10duke.com/userinfo"
    ));

    //  Default services:
    std::shared_ptr<xdhttp::HTTPClient> httpClient (new xdcurlhttp::LibCurlHTTPClient());
    std::shared_ptr<xdjson::JSONParser> jsonParser (new xdcjson::cJSONParser());
    std::shared_ptr<xdtime::Clock> clock (new xdtime::DefaultClock());
    std::shared_ptr<xdrandom::RandomBytes> randomGenerator (new xdrandom::InsecureRandomBytes());


    // Do OIDC-login:
    std::unique_ptr<xdoidc::OIDCClient> oidcClient = xdoidc::createDefaultOIDCClient(
        oauthConfig,
        oidcConfig,
        httpClient,
        jsonParser,
        randomGenerator,
        clock
    );

    std::unique_ptr<const xdoidc::OIDCRequest> oidcRequest = oidcClient->authenticate("profile email");

    std::cout << "Paste following URL to browser and navigate. Once the callback URL is navigated to, paste the " << std::endl;
    std::cout << "complete callback-URL to this terminal and press enter" << std::endl;
    std::cout << "Hint: Open dev-tools and preserve network log. In most cases you can find the callback-URL there." << std::endl;
    std::cout << "If the callback URL is not shown as a failed request, find the last redirect-response (HTTP 302):" << std::endl;
    std::cout << "the callback URL is value of response header \"Location\"" << std::endl;
    std::cout << "****" << std::endl;
    std::cout << oidcRequest->buildAuthenticationRequestUrl() << std::endl;
    std::cout << "****" << std::endl;
    std::cout << "paste callback URL here:" << std::endl;

    std::string callbackUrl;
    std::getline(std::cin, callbackUrl);

    std::unique_ptr<const xdoidc::OIDCState> oidcState = oidcRequest->handleCallback(callbackUrl);


    // Login complete, checkout licenses:
    std::shared_ptr<xdlicensing::LicensingConfiguration> licensingConfiguration (new xdlicensing::LicensingConfiguration(
        oidcState->getAccessToken(),
        "https://genco.10duke.com/authz/"
    ));

    std::unique_ptr<xdlicensing::LicensingClient> licenses (new xdlicensing::DefaultLicensingClient(
        licensingConfiguration,
        httpClient,
        jsonParser
    ));

    std::unique_ptr<xdlicensing::LicenseCheckoutResponse> checkoutResponse = licenses->checkout(xdlicensing::LicenseCheckoutParametersBuilder()
                       .hardwareId("hw-1")
                       .item("qt-demo-item-1")
                       .build()
    )->execute();

    std::cout << "Successfully checked out items:" << std::endl;
    for (auto const &item : checkoutResponse->getItems()) {
        std::cout << " - " << item.second.getName() << std::endl;
    }

    std::cout << "Failed items:" << std::endl;
    for (auto const &failedItem : checkoutResponse->getFailedItems()) {
        std::cout << " - " << failedItem.first << ":" << std::endl;
        std::cout << "   error code: " << failedItem.second.getErrorCode() << std::endl;
        std::cout << "   error key: " << failedItem.second.getErrorKey() << std::endl;
        std::cout << "   error message: " << failedItem.second.getErrorMessage() << std::endl;
        std::cout << "   tecnical: " << failedItem.second.getErrorTechnical() << std::endl;
    }
}


