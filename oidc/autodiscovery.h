#ifndef TENDUKE_OAUTH_OIDC_AUTODISCOVERY_H
#define TENDUKE_OAUTH_OIDC_AUTODISCOVERY_H

#include "./oidcconfiguration.h"

#include "crypto/publickey.h"
#include "http/httpclient.h"
#include "json/jsonobject.h"
#include "json/jsonparser.h"
#include "oauth/oauthbackendconfiguration.h"
#include "utl/base64decoder.h"

#include <memory>
#include <string>

namespace tenduke { namespace oauth { namespace oidc {

/** Result of OIDC auto-configuration query.
 */
class AutoDiscoveryResult
{
    friend class AutoDiscovery;

public:
    AutoDiscoveryResult(
        std::shared_ptr<const tenduke::oauth::OAuthBackendConfiguration> oauthConfiguration,
        std::shared_ptr<const tenduke::oauth::oidc::OIDCConfiguration> oidcConfiguration,
        const std::string &jwksDocument
    ) : oauthConfiguration(oauthConfiguration), oidcConfiguration(oidcConfiguration), jwksDocument(jwksDocument)
    {}

    std::shared_ptr<const tenduke::oauth::OAuthBackendConfiguration> getOAuthConfiguration() const {return oauthConfiguration;}

    /** Returns the discovered OIDC-configuration WITHOUT the signing key.
     *  To get the configuration WITH the signing key, use AutoDiscovery::getOIDCConfiguration()
     *
     *  @throws tenduke::crypto::CryptoException if problems with the signing key
     *  @throws tenduke::json::JSONParsingException when server response is not valid JSON
     *  @throws tenduke::oauth::oidc::OIDCException when the JWKS-document is not valid JSON
     */
    std::shared_ptr<const tenduke::oauth::oidc::OIDCConfiguration> getOOIDCConfiguration() const {return oidcConfiguration;}

private:
    const std::shared_ptr<const tenduke::oauth::OAuthBackendConfiguration> oauthConfiguration;
    const std::shared_ptr<const tenduke::oauth::oidc::OIDCConfiguration> oidcConfiguration;
    const std::string jwksDocument;
};

/** Service for querying OIDC configuration from known URL.
 */
class AutoDiscovery
{
public:
    AutoDiscovery(
            std::shared_ptr<const tenduke::utl::Base64Decoder> base64,
            std::shared_ptr<const tenduke::http::HTTPClient> http,
            std::shared_ptr<const tenduke::json::JSONParser> parseJson
    );

    /** Performs OIDC configuration auto discovery.
     *
     *  IMPORTANT NOTE:
     *
     *  In current implementation, use AutoDiscovery::getOIDCConfiguration(const AutodiscoveryResult &) to get the
     *  full OIDC-configuration including the signing key, which is parsed from the JWKS-document.
     *
     *  @param url full URL to the discovery document (e.g. https://genco.10duke.com/.well-known/openid-configuration)
     *  @return the autodiscovery result
     *  @throws tenduke::crypto::CryptoException if problems with the signing key
     *  @throws tenduke::http::HTTPException (or subclasses of it) when some HTTP-related exception (e.g. malformed URL)
     *          occurs
     *  @throws tenduke::json::JSONParsingException when server response is not valid JSON
     *  @throws tenduke::oauth::oidc::OIDCException when the server response is not what we expected, e.g.
     *          - HTTP 404 (or other HTTP-error statuses)
     *          - The response payload is not what is expected
     *  @throws tenduke::net::NetworkingException (or subclasses of it) when networking error occurred, e.g.
     *          - tenduke::net::UnknownHostException if the host is not found
     */
    virtual AutoDiscoveryResult discover(const std::string &url) const;

    /** Makes HTTP-call to given URL, and downloads the JWKS-document.
     *
     *  @param jwks_uri -
     *  @return the JWKS-document. Returns empty string, if `jwks_uri` was empty.
     *  @throws tenduke::http::HTTPException (or subclasses of it) when some HTTP-related exception (e.g. malformed URL)
     *          occurs
     *  @throws tenduke::oauth::oidc::OIDCException when the server response is not what we expected, e.g.
     *          - HTTP 404 (or other HTTP-error statuses)
     *  @throws tenduke::net::NetworkingException (or subclasses of it) when networking error occurred, e.g.
     *          - tenduke::net::UnknownHostException if the host is not found
     */
    virtual std::string downloadJWKSDocument(const std::string &jwks_uri) const;

    /** Returns the discovered OIDC-configuration, with default signing key.
     *  This method parses the JWKS-document and extracts the default signing key.
     *  To get fully ready OIDCConfiguration, use this method instead of AutoDiscoveryResult::getOIDCConfiguration().
     *
     *  @param result -
     *  @return -
     *
     *  @throws tenduke::crypto::CryptoException if problems with the signing key
     *  @throws tenduke::json::JSONParsingException when the JWKS-document is not valid JSON
     *  @throws tenduke::jwks::JWKSException when the JWKS is not ok
     */
    virtual std::shared_ptr<const tenduke::oauth::oidc::OIDCConfiguration> getOIDCConfiguration(const AutoDiscoveryResult &result) const;

protected:
    /** Extracts "default" signing key from JWKS-document.
     *
     *  @param jwksDocument the entire JWKS document
     *  @return -
     *  @throws tenduke::crypto::CryptoException if problems with the signing key
     *  @throws tenduke::json::JSONParsingException when server response is not valid JSON
     *  @throws tenduke::jwks::JWKSException when the JWKS is not ok
     */
    virtual std::unique_ptr<const tenduke::crypto::PublicKey> extractDefaultSigningKey(const std::string &jwksDocument) const;

private:
    const std::shared_ptr<const tenduke::utl::Base64Decoder> base64;
    const std::shared_ptr<const tenduke::http::HTTPClient> http;
    const std::shared_ptr<const tenduke::json::JSONParser> parseJson;
};

}}}

#endif // TENDUKE_OAUTH_OIDC_AUTODISCOVERY_H
