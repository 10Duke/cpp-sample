#ifndef OIDCCLIENT_H
#define OIDCCLIENT_H

#include "./oidcconfiguration.h"
#include "./oidcrequest.h"

#include "oauth/oauthclient.h"
#include "jwt/jwtparser.h"
#include "utl/random/randomurlsafestring.h"

#include <memory>
#include <string>

namespace tenduke { namespace oauth { namespace oidc {


// Default length of generated nonce
const size_t DEFAULT_NONCE_LENGTH = 32;


/** OpenID Connect -client.
 *
 *  This client builds OIDC-protocol on top of provided tenduke::oauth::OAuthClient.
 *
 *  Just like the OAuthClient, no browser integration is provided.
 *  See documentation of tenduke::oauth::OAuthClient.
 *
 */
class OIDCClient
{
public:
    OIDCClient(
        const std::shared_ptr<const OIDCConfiguration> oidcConfiguration,
        const std::shared_ptr<tenduke::oauth::OAuthClient> oauthClient,
        const std::shared_ptr<tenduke::utl::random::RandomUrlSafeString> randomUrlSafeString,
        const std::shared_ptr<const tenduke::jwt::JWTParser> parseJWT
    ) : oidcConfiguration(oidcConfiguration), oauthClient(oauthClient), randomUrlSafeString(randomUrlSafeString), parseJWT(parseJWT), nonceLength(DEFAULT_NONCE_LENGTH)
    {}
    virtual ~OIDCClient() = default;

    /** Creates OIDCRequest which performs the OIDC-authentication.
     *
     *  @param scopes scopes to authenticate / authorize
     *  @return the request
     */
    std::unique_ptr<const OIDCRequest> authenticate(const std::string &scopes);

protected:
    std::string generateNonce();

private:
    const std::shared_ptr<const OIDCConfiguration> oidcConfiguration;
    const std::shared_ptr<tenduke::oauth::OAuthClient> oauthClient;
    const std::shared_ptr<tenduke::utl::random::RandomUrlSafeString> randomUrlSafeString;
    const std::shared_ptr<const tenduke::jwt::JWTParser> parseJWT;

    const size_t nonceLength;
};


}}}

#endif // OIDCCLIENT_H
