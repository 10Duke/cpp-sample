#include "./createdefaultoidcclient.h"

#include "crypto/asymmetricmessagedigestfactory.h"
#include "crypto/rsapublickeyfrompemstring.h"
#include "http/libcurlhttpclient.h"
#include "json/cjsonparser.h"
#include "jwt/defaultjwtparser.h"
#include "jwt/fixedkeyjwtsignaturevalidator.h"
#include "net/libcurlurlparser.h"
#include "oauth/oauthpkceflow.h"
#include "oidc/oidcclient.h"
#include "time/defaultclock.h"
#include "utl/defaultbase64decoder.h"
#include "utl/defaultbase64encoder.h"
#include "utl/random/insecurerandombytes.h"
#include "utl/random/randomurlsafestring.h"


namespace crypto = tenduke::crypto;
namespace libcrypto = tenduke::crypto::libcrypto;
namespace jwt = tenduke::jwt;
namespace rnd = tenduke::utl::random;
namespace xdcjson = tenduke::json::cjson;
namespace xdcurlhttp = tenduke::http::curl;
namespace xdcurlnet = tenduke::net::curl;
namespace xdjson = tenduke::json;
namespace xdhttp = tenduke::http;
namespace xdnet = tenduke::net;
namespace xdoauth = tenduke::oauth;
namespace xdoidc = tenduke::oauth::oidc;
namespace xdutl = tenduke::utl;
namespace xdtime = tenduke::time;



std::unique_ptr<xdoidc::OIDCClient> xdoidc::createDefaultOIDCClient(
    const std::shared_ptr<const xdoauth::OAuthConfiguration> oauthConfiguration,
    const std::shared_ptr<const xdoidc::OIDCConfiguration> oidcConfiguration,
    const std::shared_ptr<xdhttp::HTTPClient> httpClient,
    const std::shared_ptr<const xdjson::JSONParser> jsonParser,
    const std::shared_ptr<rnd::RandomBytes> randomGenerator,
    const std::shared_ptr<xdtime::Clock> clock
)
{
    // Lots of services used intentionally: These enable customization, library-specific implementations and (hopefully) easier testing.

    std::shared_ptr<rnd::RandomUrlSafeString> randomString (new rnd::RandomUrlSafeString(randomGenerator));
    std::shared_ptr<const xdutl::Base64Decoder> base64Decoder (new xdutl::DefaultBase64Decoder());
    std::shared_ptr<const xdutl::Base64Encoder> base64Encoder (new xdutl::DefaultBase64Encoder());
    std::shared_ptr<const xdnet::URLParser> urlParser (new xdcurlnet::LibCurlURLParser());

    // OAuth:
    std::shared_ptr<xdoauth::OAuthClient> oauthClient (new xdoauth::OAuthPKCEFlow(
        oauthConfiguration,
        httpClient,
        urlParser,
        jsonParser,
        base64Encoder,
        clock,
        randomString
    ));

    // JWT-parsing:
    std::shared_ptr<const crypto::MessageDigestVerifierFactory> digestFactory(new libcrypto::AsymmetricMessageDigestFactory(
        oidcConfiguration->algorithm,
        oidcConfiguration->verificationKey
    ));
    std::shared_ptr<const jwt::JWTSignatureValidator> jwtSignatureValidator (new jwt::FixedKeyJWTSignatureValidator(digestFactory));
    std::shared_ptr<const jwt::JWTParser> jwtParser (new jwt::DefaultJWTParser(
        jsonParser,
        base64Decoder,
        jwtSignatureValidator
    ));

    return std::unique_ptr<xdoidc::OIDCClient>(new xdoidc::OIDCClient(
        oidcConfiguration,
        oauthClient,
        randomString,
        jwtParser
    ));
}
