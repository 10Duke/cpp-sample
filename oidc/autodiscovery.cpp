#include "autodiscovery.h"

#include "crypto/publickey.h"
#include "json/jsonutils.h"
#include "jwks/jwkstopublickey.h"
#include "oidc/oidcexception.h"

namespace crypto = tenduke::crypto;
namespace http = tenduke::http;
namespace json = tenduke::json;
namespace jwks = tenduke::jwks;
namespace oauth = tenduke::oauth;
namespace oidc = tenduke::oauth::oidc;
namespace utl = tenduke::utl;


oidc::AutoDiscovery::AutoDiscovery(
    std::shared_ptr<const tenduke::utl::Base64Decoder> base64,
    std::shared_ptr<const tenduke::http::HTTPClient> http,
    std::shared_ptr<const tenduke::json::JSONParser> parseJson
) : base64(base64), http(http), parseJson(parseJson)
{
}


oidc::AutoDiscoveryResult oidc::AutoDiscovery::discover(const std::string &url)
const
{
    std::shared_ptr<http::HTTPRequest> request = http->request().get().url(url).build();
    std::unique_ptr<http::HTTPCall> call = http->call(request);
    std::unique_ptr<http::HTTPResponse> response = call->execute();

    if (! response->isSuccessful()) {
        throw OIDCException("Auto-discovery HTTP-request failed with HTTP status code " + std::to_string(response->getStatusCode()));
    }

    std::unique_ptr<json::JSONElement> json = parseJson->from(response->getPayloadAsString());

    if (! json->isObject()) {
        throw OIDCException("The discovery document is not JSON object");
    }

    // Extract the required properties from the discovery document:
    const json::JSONObject* document = dynamic_cast<const json::JSONObject *>(json.get());
    const std::string issuer = json::getObjectPropertyAsString(document, "issuer");
    const std::string authorizationEndpoint = json::getObjectPropertyAsString(document, "authorization_endpoint");
    const std::string tokenEndpoint = json::getObjectPropertyAsString(document, "token_endpoint");
    const std::string userinfoEndpoint = json::getObjectPropertyAsString(document, "userinfo_endpoint");
    const std::string jwksUri = json::getObjectPropertyAsString(document, "jwks_uri");

    return AutoDiscoveryResult(
        std::unique_ptr<oauth::OAuthBackendConfiguration>(new OAuthBackendConfiguration (
            authorizationEndpoint,
            tokenEndpoint
        )),
        std::unique_ptr<oidc::OIDCConfiguration>(new OIDCConfiguration (
            issuer,
            nullptr, // nullptr is intentional: We do not convert the JWKS to key, because the result could be serialized (eq. QT signals)
                     // and the semantics of that are slightly unclear, as the public key holds a handle to LibCrypto key
            userinfoEndpoint
        )),
        (jwksUri.empty() ? "" : downloadJWKSDocument(jwksUri))
    );
}


std::string oidc::AutoDiscovery::downloadJWKSDocument(const std::string &jwks_uri)
const
{
    std::shared_ptr<http::HTTPRequest> request = http->request().get().url(jwks_uri).build();
    std::unique_ptr<http::HTTPCall> call = http->call(request);
    std::unique_ptr<http::HTTPResponse> response = call->execute();

    // ERROR: HTTP-status >= 400
    if (! response->isSuccessful()) {
        throw OIDCException("JWKS-request to URL " + jwks_uri + " failed with status code " + std::to_string(response->getStatusCode()));
    }

    return response->getPayloadAsString();
}


std::shared_ptr<const oidc::OIDCConfiguration> oidc::AutoDiscovery::getOIDCConfiguration(const oidc::AutoDiscoveryResult &result)
const
{
    const std::shared_ptr<const oidc::OIDCConfiguration> original = result.getOOIDCConfiguration();

    return std::shared_ptr<const oidc::OIDCConfiguration>(new OIDCConfiguration(
        original->issuer,
        (result.jwksDocument.empty() ? nullptr : extractDefaultSigningKey(result.jwksDocument)),
        original->userinfoEndpoint
    ));
}


std::unique_ptr<const crypto::PublicKey> oidc::AutoDiscovery::extractDefaultSigningKey(const std::string &jwksDocument)
const
{
    jwks::JWKSToPublicKey createPublicKey(parseJson, base64);

    return createPublicKey.from(jwksDocument);
}



