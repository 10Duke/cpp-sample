#ifndef OIDCREQUEST_H
#define OIDCREQUEST_H

#include "./oidcconfiguration.h"
#include "./oidcstate.h"

#include "jwt/jwtparser.h"
#include "oauth/oauthrequest.h"

#include <memory>
#include <string>

namespace tenduke { namespace oauth { namespace oidc {


/** OIDC-request, which handles the OIDC-flow.
 *
 *  Very much like tenduke::oauth::OAuthRequest, see the documentation of both
 *  tenduke::oauth::OAuthClient and tenduke::oauth::OAuthRequest for details.
 *
 */
class OIDCRequest
{
public:
    OIDCRequest(
            std::unique_ptr<const tenduke::oauth::OAuthRequest> oauthRequest,
            const std::shared_ptr<const OIDCConfiguration> oidcConfiguration,
            const std::string nonce,
            const std::shared_ptr<const tenduke::jwt::JWTParser> parseJWT
    ) : oauthRequest(std::move(oauthRequest)), nonce(nonce), oidcConfiguration(oidcConfiguration), parseJWT(parseJWT)
    {}
    virtual ~OIDCRequest() {}

    /** Creates the initial authenticationrequest URL.
     *  Navigate browser to this URL.
     */
    virtual std::string buildAuthenticationRequestUrl() const;

    /** Handles callback: performs consequent actions to authenticate.
     *
     *  See documentation of method tenduke::oauth::OAuthRequest::handleCallback().
     *
     *  The call is synchronous.
     *
     *  @throws std::nested_exception when the there is some nested cause
     *  @throws tenduke::http::HTTPException (or subclasses of it) when the server responds with HTTP error status code
     *          which does not indicate OAuth-error, e.g. 500 (internal server error) or 404 (not found = wrong endpoint
     *          configured)
     *  @throws tenduke::json::JSONParsingException when server response is not valid JSON
     *  @throws tenduke::oauth::OAuthException when the server response is not what we expected, e.g.
     *          - problems with the callback-URL (missing or invalid "state", missing "code")
     *          - The token response does not contain access_token
     *          - The token response payload is not JSON-object
     *  @throws tenduke::oauth::OAuthServerError when the server responds with an error
     *  @throws tenduke::net::NetworkingException (or subclasses of it) when networking error occurred, e.g.
     *          - tenduke::net::UnknownHostException if the host is not found
     *  @throws tenduke::net::URLException if problem parsing the callback-URL (should not happen, though)
     *  @throws tenduke::net::MalformedURLException if the callback-URL is invalid
     */
    virtual std::unique_ptr<OIDCState> handleCallback(const std::string &callbackUrl) const;

protected:
    virtual void validateIdToken(const std::string &idToken) const;

private:
    const std::unique_ptr<const tenduke::oauth::OAuthRequest> oauthRequest;
    const std::string nonce;
    const std::shared_ptr<const OIDCConfiguration> oidcConfiguration;
    const std::shared_ptr<const tenduke::jwt::JWTParser> parseJWT;
};


}}}

#endif // OIDCREQUEST_H
