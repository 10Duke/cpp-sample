#ifndef OAUTHPKCEREQUEST_H
#define OAUTHPKCEREQUEST_H

#include "./oauthconfiguration.h"
#include "./oauthrequest.h"
#include "./oauthtokenrequest.h"
#include "./oauthstate.h"

#include "http/httpclient.h"
#include "json/jsonparser.h"
#include "net/urlparser.h"
#include "time/clock.h"
#include "utl/base64encoder.h"

#include <map>
#include <memory>

namespace tenduke { namespace oauth {


/** PKCE-implementation of tenduke::oauth::OAuthRequest.
 *
 */
class OAuthPKCERequest : public OAuthRequest
{
public:
    OAuthPKCERequest(
            const std::string scopes,
            const std::string state,
            const std::map<std::string, std::string> additionalParameters,
            const std::string codeVerifier,
            const std::shared_ptr<const OAuthConfiguration> config,
            const std::shared_ptr<const tenduke::http::HTTPClient> httpClient,
            const std::shared_ptr<const tenduke::net::URLParser> urlParser,
            const std::shared_ptr<const tenduke::json::JSONParser> jsonParser,
            const std::shared_ptr<const tenduke::utl::Base64Encoder> base64Encoder,
            const std::shared_ptr<tenduke::time::Clock> clock
    );
    virtual ~OAuthPKCERequest() {}

    // tenduke::oauth::OAuthRequest interface
    virtual std::string buildAuthorizationRequestUrl() const override;
    virtual std::unique_ptr<OAuthState> handleCallback(const std::string &callbackUrl) const override;

protected:
    virtual std::map<std::string, std::string> buildTokenRequestParameters(const std::string &code) const;
    virtual std::map<std::string, std::string> buildAuthorizationRequestParameters() const;
    virtual void createCodeChallenge(std::map<std::string, std::string> &parameters) const;
    virtual std::unique_ptr<OAuthState> handleSuccessfulCallback(const std::multimap<std::string, std::string> &queryParameters) const;
    virtual std::unique_ptr<OAuthState> handleTokenResponse(std::unique_ptr<tenduke::oauth::OAuthTokenResponse> response) const;
    virtual bool isPKCERequest() const;
    virtual std::unique_ptr<OAuthTokenRequest> createTokenRequest(const std::string &code) const;

private:
    const std::string scopes;
    const std::string state;
    const std::map<std::string, std::string> additionalParameters;

    const std::string codeVerifier; // PKCE code verifier

    const std::shared_ptr<const OAuthConfiguration> config;
    const std::shared_ptr<const tenduke::http::HTTPClient> http;
    const std::shared_ptr<const tenduke::net::URLParser> parseURL;
    const std::shared_ptr<const tenduke::json::JSONParser> parseJSON;
    const std::shared_ptr<const tenduke::utl::Base64Encoder> base64Encoder;
    const std::shared_ptr<tenduke::time::Clock> clock;
};


}}

#endif // OAUTHPKCEREQUEST_H
