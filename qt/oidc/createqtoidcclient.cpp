#include "createqtoidcclient.h"

#include "crypto/asymmetricmessagedigestfactory.h"
#include "crypto/rsapublickeyfrompemstring.h"
#include "jwt/createjwtparser.h"
#include "jwt/jwtparser.h"
#include "jwt/fixedkeyjwtsignaturevalidator.h"
#include "oauth/oauthpkceflow.h"
#include "time/defaultclock.h"
#include "qt/json/qtjsonparser.h"
#include "qt/net/qturlparser.h"
#include "qt/utl/qtbase64decoder.h"
#include "qt/utl/qtbase64encoder.h"
#include "qt/utl/random/qtrandombytes.h"
#include "utl/random/randombytes.h"
#include "utl/random/randomurlsafestring.h"


namespace crypto = tenduke::crypto;
namespace libcrypto = tenduke::crypto::libcrypto;
namespace http = tenduke::http;
namespace qtnet = tenduke::qt::net;
namespace qtoidc = tenduke::qt::oidc;
namespace qtutl = tenduke::qt::utl;
namespace qtjson = tenduke::qt::json;
namespace rnd = tenduke::utl::random;
namespace xdjson = tenduke::json;
namespace xdjwt = tenduke::jwt;
namespace xdnet = tenduke::net;
namespace xdoauth = tenduke::oauth;
namespace xdoidc = tenduke::oauth::oidc;
namespace xdutl = tenduke::utl;
namespace xdtime = tenduke::time;

std::unique_ptr<const tenduke::oauth::oidc::AutoDiscovery> qtoidc::createAutoDiscovery(std::shared_ptr<const tenduke::http::HTTPClient> httpClient)
{
    return std::unique_ptr<const xdoidc::AutoDiscovery>(new xdoidc::AutoDiscovery(
        std::shared_ptr<const xdutl::Base64Decoder>(new qtutl::QtBase64Decoder()),
        httpClient,
        std::shared_ptr<const xdjson::JSONParser>(new qtjson::QtJSONParser())
    ));
}


std::unique_ptr<qtoidc::QtOIDCClient> qtoidc::createQtOIDCClient(
    std::shared_ptr<const xdoauth::OAuthConfiguration> oauthConfiguration,
    std::shared_ptr<const xdoidc::OIDCConfiguration> oidcConfiguration,
    std::shared_ptr<const http::HTTPClient> httpClient,
    std::shared_ptr<const xdjson::JSONParser> jsonParser,
    std::shared_ptr<xdtime::Clock> clock
) {
    // Lots of services used intentionally: These enable customization, library-specific implementations and (hopefully) easier testing.
    std::shared_ptr<rnd::RandomBytes> randomBytes = std::shared_ptr<rnd::RandomBytes>(new qtutl::QtRandomBytes());
    std::shared_ptr<rnd::RandomUrlSafeString> randomString = std::shared_ptr<rnd::RandomUrlSafeString>(new rnd::RandomUrlSafeString(randomBytes));
    std::shared_ptr<const xdutl::Base64Decoder> base64Decoder (new qtutl::QtBase64Decoder());
    std::shared_ptr<const xdutl::Base64Encoder> base64Encoder (new qtutl::QtBase64Encoder());
    std::shared_ptr<const xdnet::URLParser> urlParser (new qtnet::QtURLParser());

    // OAuth:
    std::shared_ptr<xdoauth::OAuthClient> oauthClient = std::shared_ptr<xdoauth::OAuthClient>(new xdoauth::OAuthPKCEFlow(
        oauthConfiguration,
        httpClient,
        urlParser,
        jsonParser,
        base64Encoder,
        clock,
        randomString
    ));

    // JWT-parsing:
    std::shared_ptr<const xdjwt::JWTParser> jwtParser = xdjwt::createJWTParser(
        oidcConfiguration->algorithm,
        oidcConfiguration->verificationKey,
        base64Decoder,
        jsonParser
    );

    std::shared_ptr<xdoidc::OIDCClient> oidcClient = std::shared_ptr<xdoidc::OIDCClient>(new xdoidc::OIDCClient(
        oidcConfiguration,
        oauthClient,
        randomString,
        jwtParser
    ));

    return std::unique_ptr<qtoidc::QtOIDCClient>(new qtoidc::QtOIDCClient(
        oidcClient
    ));
}

std::unique_ptr<qtoidc::QtOIDCClient> qtoidc::createQtOIDCClient(
    std::shared_ptr<const xdoauth::OAuthConfiguration> oauthConfiguration,
    std::shared_ptr<const xdoidc::OIDCConfiguration> oidcConfiguration,
    std::shared_ptr<const http::HTTPClient> httpClient
) {
    return qtoidc::createQtOIDCClient(
        oauthConfiguration,
        oidcConfiguration,
        httpClient,
        std::shared_ptr<xdjson::JSONParser>(new qtjson::QtJSONParser()),
        std::shared_ptr<xdtime::Clock>(new xdtime::DefaultClock())
    );
}

static std::unique_ptr<const crypto::PublicKey> createPublicKey(QString key)
{
    if (key.isEmpty()) {
        return std::unique_ptr<const crypto::PublicKey>();
    }

    // NOTE: This will throw CryptoException if the key is invalid.
    return libcrypto::RSAPublicKeyFromPEMString().from(key.toStdString());
}

std::shared_ptr<qtoidc::QtOIDCClient> qtoidc::createOidcClient(
        std::shared_ptr<const qtoidc::QtOIDCConfig> oidcConfiguration,
        std::shared_ptr<const http::HTTPClient> httpClient
) {
    // Construct public key:
    std::unique_ptr<const crypto::PublicKey> idTokenValidationKey = createPublicKey(oidcConfiguration->verificationKey);

    // Create QtOIDCClient
    return std::unique_ptr<qtoidc::QtOIDCClient> (qtoidc::createQtOIDCClient(
                std::shared_ptr<xdoauth::OAuthConfiguration>(new xdoauth::OAuthConfiguration(
                    oidcConfiguration->getAuthorizationEndpoint().toString().toStdString(),
                    oidcConfiguration->getTokenEndpoint().toString().toStdString(),
                    oidcConfiguration->getClientIdentifier().toStdString(),
                    oidcConfiguration->getClientSecret().toStdString(),
                    oidcConfiguration->getRedirectUri().toStdString(),
                    true
                )),
                std::shared_ptr<xdoidc::OIDCConfiguration>(new xdoidc::OIDCConfiguration(
                        oidcConfiguration->issuer.toStdString(),
                        std::move(idTokenValidationKey),
                        oidcConfiguration->getUserinfoEndpoint().toString().toStdString()
                )),
                httpClient
    ));
}


