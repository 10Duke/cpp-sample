#ifndef OAUTHREQUEST_H
#define OAUTHREQUEST_H

#include "./oauthstate.h"

#include <map>
#include <memory>
#include <string>

namespace tenduke { namespace oauth {


/** OAuth-request handles the OAuth authorization flow.
 *
 */
class OAuthRequest
{
public:
    virtual ~OAuthRequest() {}

    /** Creates the initial authorization request URL.
     *  Navigate browser to this URL.
     */
    virtual std::string buildAuthorizationRequestUrl() const = 0;

    /** Handles callback: performs consequent actions to authorize.
     *
     *  In the flow, the OAuth-server signals success or error by navigating (using HTTP-redirect)
     *  to configured callback URL. You must listen the browser events to detect, when the callback-URL
     *  is navigated to. Once you detect navigation to the callback-URL, call this method with the complete
     *  callback URL.
     *
     *  The call is synchronous.
     *
     *  @return the OAuth-state
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
    virtual std::unique_ptr<OAuthState> handleCallback(const std::string &callbackUrl) const = 0;
};


}}

#endif // OAUTHREQUEST_H
