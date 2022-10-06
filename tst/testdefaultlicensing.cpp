#include "./testdefaultlicensing.h"

#include "crypto/rsapublickeyfrompemstring.h"
#include "crypto/publickeytopem.h"
#include "http/libcurlhttpclient.h"
#include "json/cjsonparser.h"
#include "licensing/defaultlicensingclient.h"
#include "licensing/licensecheckoutparametersbuilder.h"
#include "oauth/oauthclientconfiguration.h"
#include "oauth/oauthconfiguration.h"
#include "oidc/autodiscovery.h"
#include "oidc/createdefaultoidcclient.h"
#include "oidc/oidcconfiguration.h"
#include "oidc/oidcclient.h"
#include "time/defaultclock.h"
#include "utl/random/insecurerandombytes.h"
#include "utl/defaultbase64decoder.h"

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
namespace xdutl = tenduke::utl;

// IMPORTANT: Change following configuration items to match the deployment
static const std::string HOST="https://genco.10duke.com";
static const std::string OAUTH_CLIENT_ID = "QTDemo";
static const std::string OAUTH_CLIENT_SECRET = "FQdL!XSYYrQ7-zeiCe";
static const std::string OAUTH_CALLBACK_URI = "tenduke://callback";
static const bool USE_PKCE = true;
static const std::string LICENSED_ITEM_NAME = "qt-demo-item-1";
static const std::string HARDWARE_ID = "hw-1";

// Usually no need to change these
static const std::string OIDC_AUTO_DISCOVERY_URL = HOST + "/.well-known/openid-configuration";
static const std::string LICENSING_API_ENDPOINT = HOST + "/authz/";



void tenduke::tst::licensing::testDefaultLicensingWithAutoDiscovery()
{
    //  Default services:
    std::shared_ptr<xdhttp::HTTPClient> httpClient (new xdcurlhttp::LibCurlHTTPClient());
    std::shared_ptr<xdjson::JSONParser> jsonParser (new xdcjson::cJSONParser());
    std::shared_ptr<xdtime::Clock> clock (new xdtime::DefaultClock());
    std::shared_ptr<xdrandom::RandomBytes> randomGenerator (new xdrandom::InsecureRandomBytes());
    std::shared_ptr<xdutl::Base64Decoder> base64Decoder (new xdutl::DefaultBase64Decoder());

    // Auto-discover OIDC backend configuration
    xdoidc::AutoDiscovery oidcConfiguration(base64Decoder, httpClient, jsonParser);
    xdoidc::AutoDiscoveryResult discoveredCfg = oidcConfiguration.discover(OIDC_AUTO_DISCOVERY_URL);

    // IMPORTANT: This constructs the OIDC-backend configuration, INCLUDING the verification key
    // The discovery result does not contain parsed verification key, but contains the JWKS-document instead.
    std::shared_ptr<const xdoidc::OIDCConfiguration> discoveredOIDCCfg = oidcConfiguration.getOIDCConfiguration(discoveredCfg);
    libcrypto::PublicKeyToPEM createPEM;

    std::cout << "Auto discovered properties:" << std::endl;
    std::cout << " - authorization endpoint: " << discoveredCfg.getOAuthConfiguration()->authorizationEndpointUrl << std::endl;
    std::cout << " - token endpoint:         " << discoveredCfg.getOAuthConfiguration()->tokenEndpointUrl << std::endl;
    std::cout << " - userinfo endpoint:      " << discoveredCfg.getOOIDCConfiguration()->userinfoEndpoint << std::endl;
    std::cout << " - issuer:                 " << discoveredCfg.getOOIDCConfiguration()->issuer<< std::endl;
    std::cout << " - verification key:" << std::endl;
    std::cout << createPEM.from(*(discoveredOIDCCfg->verificationKey.get()));
    std::cout << std::endl << std::endl;;

    // Make the OAuth-config from the auto-discovered backend configuration plus the client configuration:
    std::shared_ptr<const xdoauth::OAuthConfiguration> oauthConfig (new xdoauth::OAuthConfiguration(
        *discoveredCfg.getOAuthConfiguration().get(),
        xdoauth::OAuthClientConfiguration(
            OAUTH_CLIENT_ID,
            OAUTH_CLIENT_SECRET,
            OAUTH_CALLBACK_URI,
            USE_PKCE
        )
    ));

    // Do OIDC-login:
    std::unique_ptr<xdoidc::OIDCClient> oidcClient = xdoidc::createDefaultOIDCClient(
        oauthConfig,
        discoveredOIDCCfg,
        httpClient,
        jsonParser,
        randomGenerator,
        clock
    );

    std::unique_ptr<const xdoidc::OIDCRequest> oidcRequest = oidcClient->authenticate("profile email");

    std::cout << "Paste following URL to browser and navigate. Once the callback URL is navigated to, paste the " << std::endl;
    std::cout << "complete callback-URL to this terminal and press enter." << std::endl << std::endl;
    std::cout << "HINT: Before you paste the link, open dev-tools and preserve network log. In most cases you" << std::endl;
    std::cout << "can find the callback-URL there. If the callback URL is not shown as a failed request, find" << std::endl;
    std::cout << "the last redirect-response (HTTP 302): the callback URL is value of response header \"Location\"" << std::endl <<std::endl;
    std::cout << "****" << std::endl;
    std::cout << oidcRequest->buildAuthenticationRequestUrl() << std::endl;
    std::cout << "****" << std::endl << std::endl;
    std::cout << "paste callback URL (starting with \"" << OAUTH_CALLBACK_URI << "\") here:" << std::endl;

    std::string callbackUrl;
    std::getline(std::cin, callbackUrl);

    std::unique_ptr<const xdoidc::OIDCState> oidcState = oidcRequest->handleCallback(callbackUrl);

    std::cout << std::endl << "Login successful." << std::endl << std::endl;
    std::cout << "Checking out licenses..." << std::endl << std::endl;

    // Login complete, checkout licenses:
    std::shared_ptr<xdlicensing::LicensingConfiguration> licensingConfiguration (new xdlicensing::LicensingConfiguration(
        oidcState->getAccessToken(),
        LICENSING_API_ENDPOINT
    ));

    std::unique_ptr<xdlicensing::LicensingClient> licenses (new xdlicensing::DefaultLicensingClient(
        licensingConfiguration,
        httpClient,
        jsonParser
    ));

    std::unique_ptr<xdlicensing::LicenseCheckoutResponse> checkoutResponse = licenses->checkout(xdlicensing::LicenseCheckoutParametersBuilder()
                       .hardwareId(HARDWARE_ID)
                       .item(LICENSED_ITEM_NAME)
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
