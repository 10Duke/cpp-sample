#ifndef OAUTHPKCEFLOW_H
#define OAUTHPKCEFLOW_H

#include "./oauthclient.h"
#include "./oauthconfiguration.h"
#include "./oauthrequest.h"

#include "http/httpclient.h"
#include "json/jsonparser.h"
#include "net/urlparser.h"
#include "time/clock.h"
#include "utl/base64encoder.h"
#include "utl/random/randomurlsafestring.h"

#include <stddef.h>
#include <map>
#include <memory>
#include <string>

namespace tenduke { namespace oauth {


// Default length of generated state-parameter
const size_t DEFAULT_STATE_LENGTH = 16;
// Default length of generated PKCE code verifier
const size_t DEFAULT_CODEVERIFIER_LENGTH = 64;


/** OAuth Authorization Code Grant with PKCE implementation of tenduke::oauth::OAuthClient.
 *
 */
class OAuthPKCEFlow : public tenduke::oauth::OAuthClient
{
public:
    OAuthPKCEFlow(
            const std::shared_ptr<const OAuthConfiguration> config,
            const std::shared_ptr<const tenduke::http::HTTPClient> httpClient,
            const std::shared_ptr<const tenduke::net::URLParser> urlParser,
            const std::shared_ptr<const tenduke::json::JSONParser> jsonParser,
            const std::shared_ptr<const tenduke::utl::Base64Encoder> base64Encoder,
            const std::shared_ptr<tenduke::time::Clock> clock,
            const std::shared_ptr<tenduke::utl::random::RandomUrlSafeString> randomUrlSafeString
    );

    // tenduke::oauth::OAuthClient interface
    virtual std::unique_ptr<const tenduke::oauth::OAuthRequest> authorize(
            const std::string &scopes,
            const std::map<std::string, std::string> &additionalParameters
    ) override;

public:
    const std::shared_ptr<const OAuthConfiguration> getConfiguration() const {return config;}

protected:
    std::string generateNonce();
    std::string generateState();
    std::string generateCodeVerifier();

private:
    size_t stateLength;
    size_t pkceCodeVerifierLength;

    const std::shared_ptr<const OAuthConfiguration> config;
    const std::shared_ptr<const tenduke::http::HTTPClient> httpClient;
    const std::shared_ptr<const tenduke::net::URLParser> urlParser;
    const std::shared_ptr<const tenduke::json::JSONParser> jsonParser;
    const std::shared_ptr<const tenduke::utl::Base64Encoder> base64Encoder;
    const std::shared_ptr<tenduke::time::Clock> clock;
    const std::shared_ptr<tenduke::utl::random::RandomUrlSafeString> randomUrlSafeString;
};


}}

#endif // OAUTHPKCEFLOW_H
