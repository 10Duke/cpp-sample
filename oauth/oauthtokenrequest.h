#ifndef OAUTHTOKENREQUEST_H
#define OAUTHTOKENREQUEST_H

#include "./oauthtokenresponse.h"
#include "http/httpcall.h"
#include "http/httpclient.h"
#include "http/httpstatuscodetoexception.h"
#include "json/jsonobject.h"
#include "json/jsonparser.h"
#include "time/clock.h"

#include <memory>

namespace tenduke { namespace oauth {


/** OAuth token request.
 *  Performs both "exchange-code-to-accesstoken" and "refresh token" requests.
 *  Used mostly internally to share common functionality.
 *
 */
class OAuthTokenRequest
{
public:
    OAuthTokenRequest(
        std::unique_ptr<tenduke::http::HTTPCall> httpCall,
        std::shared_ptr<const tenduke::json::JSONParser> jsonParser,
        std::shared_ptr<tenduke::time::Clock> clock
    );

    virtual ~OAuthTokenRequest() {}

    /** Executes the token request.
     *
     *  @throws tenduke::oauth::OAuthServerError when the server responds with an error
     *  @throws tenduke::oauth::OAuthException when the server response is not what we expected, e.g.
     *          - The response does not contain access_token
     *          - The response payload is not JSON-object
     *  @throws tenduke::json::JSONParsingException when response is not valid JSON
     *  @throws tenduke::net::NetworkingException (or subclasses of it) when networking error occurred, e.g.
     *          - tenduke::net::UnknownHostException if the host is not found
     *  @throws tenduke::http::HTTPException (or subclasses of it) when the server responds with HTTP error status code
     *          which does not indicate OAuth-error, e.g. 500 (internal server error) or 404 (not found = wrong endpoint
     *          configured).
     *  @throws std::nested_exception when the there is some nested cause
     */
    virtual std::unique_ptr<tenduke::oauth::OAuthTokenResponse> execute() const;

    /** Factory method to create the request.
     *
     */
    static std::unique_ptr<OAuthTokenRequest> create(
            const std::string &tokenEndpointUrl,
            const std::map<std::string, std::string> &parameters,
            const std::shared_ptr<const tenduke::http::HTTPClient> httpClient,
            const std::shared_ptr<const tenduke::json::JSONParser> jsonParser,
            const std::shared_ptr<tenduke::time::Clock> clock
    );

protected:
    virtual std::unique_ptr<tenduke::json::JSONObject> parseResponseBody(const std::string &responsePayload) const;
    virtual std::unique_ptr<tenduke::oauth::OAuthTokenResponse> toResponse(const std::string &responsePayload) const;

private:
    const std::unique_ptr<tenduke::http::HTTPCall> httpCall;
    const std::shared_ptr<const tenduke::json::JSONParser> parseJSON;
    const std::shared_ptr<const tenduke::time::Clock> clock;
    const std::shared_ptr<const tenduke::http::HTTPStatusCodeToException> throwException;
};


}}

#endif // OAUTHTOKENREQUEST_H
