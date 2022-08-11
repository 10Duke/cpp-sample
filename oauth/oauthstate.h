#ifndef OAUTHSTATE_H
#define OAUTHSTATE_H

#include <cstdint>
#include <map>
#include <string>

namespace tenduke { namespace oauth {


// Magic value to denote that the access token does not expire
const std::int64_t TOKEN_DOES_NOT_EXPIRE = -1;


/** OAuth-state after successful authentication.
 *
 */
class OAuthState
{
public:
    virtual ~OAuthState() {}

    /** Returns the access token. */
    virtual const std::string & getAccessToken() const = 0;

    /** Returns the refresh token.
     *
     *  @return the refresh-token. Returns empty string if no refresh token.
     */
    virtual const std::string & getRefreshToken() const = 0;

    /** Returns timestamp (as epoch seconds) when the current access token expires.
     *
     *  @return expiration of current accesstime in epoch seconds.
     *          Returns tenduke::oauth::TOKEN_DOES_NOT_EXPIRE if server did not return "expires_at".
     */
    virtual std::int64_t getExpiresAt() const = 0;

    /** Returns additional property returned by the server.
     *
     *  @param name property name
     *  @return property value or nullptr if no such property
     */
    virtual const std::string * getAdditionalProperty(const std::string &name) const = 0;

    /** Refresh the state, i.e. requests new access token.
     *  The refresh is done synchronously.
     *
     *  @return {@code true} if refreshed.
     *          Returns {@code false} if nothing was done (because the state does not contain refresh token).
     *
     *  @throws tenduke::oauth::OAuthServerException when the server responds with an error
     *  @throws tenduke::oauth::OAuthException when the server response is not what we expected, e.g.
     *          - The response does not contain access_token
     *          - The response payload is not JSON-object
     *  @throws tenduke::json::JSONParsingException when response is not valid JSON
     *  @throws tenduke::net::NetworkingException (or subclasses of it) when networking error occurred, e.g.
     *          - tenduke::net::UnknownHostException if the host is not found
     *  @throws tenduke::http::HTTPException (or subclasses of it) when the server responds with HTTP error status code
     *          which does not indicate OAuth-error, e.g. 500 (internal server error) or 404 (not found = wrong endpoint
     *          configured).
     */
    virtual bool refresh() = 0;
};


}}

#endif // OAUTHSTATE_H

