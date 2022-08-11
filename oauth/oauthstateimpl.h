#ifndef OAUTHSTATEIMPL_H
#define OAUTHSTATEIMPL_H

#include "./oauthstate.h"
#include "./oauthtokenrequest.h"
#include "./oauthconfiguration.h"

#include "http/httpclient.h"
#include "json/jsonparser.h"
#include "time/clock.h"

#include <memory>


namespace tenduke { namespace oauth {


/** Default implementation of tenduke::oauth::OAuthState.
 *
 */
class OAuthStateImpl : public OAuthState
{
public:
    OAuthStateImpl(
            std::string accessToken,
            std::string refreshToken,
            int64_t expiresAt,
            const std::map<std::string, std::string> additionalProperties,
            const std::shared_ptr<const OAuthConfiguration> config,
            const std::shared_ptr<const tenduke::http::HTTPClient> httpClient,
            const std::shared_ptr<const tenduke::json::JSONParser> jsonParser,
            const std::shared_ptr< tenduke::time::Clock> clock
    ) : accessToken(accessToken), refreshToken(refreshToken), expiresAt(expiresAt), additionalProperties(additionalProperties),
        config(config), http(httpClient), parseJSON(jsonParser), clock(clock)
    {}

    // tenduke::oauth::OAuthState interface
    virtual const std::string & getAccessToken() const override {return accessToken;}
    virtual const std::string & getRefreshToken() const override {return refreshToken;}
    virtual int64_t getExpiresAt() const override {return expiresAt;}
    virtual const std::string * getAdditionalProperty(const std::string &name) const override;
    virtual bool refresh() override;

protected:
    virtual std::map<std::string, std::string> buildRefreshRequestParameters() const;
    virtual std::unique_ptr<OAuthTokenRequest> createRefreshTokenRequest() const;

private:
    std::string accessToken;
    std::string refreshToken;
    int64_t expiresAt;

    std::map<std::string, std::string> additionalProperties;

    const std::shared_ptr<const OAuthConfiguration> config;
    const std::shared_ptr<const tenduke::http::HTTPClient> http;
    const std::shared_ptr<const tenduke::json::JSONParser> parseJSON;
    const std::shared_ptr<tenduke::time::Clock> clock;
};


}}

#endif // OAUTHSTATEIMPL_H
