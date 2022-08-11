#include "./oauthpkceflow.h"
#include "./oauthpkcerequest.h"

#include "crypto/sha256utl.h"

namespace http = tenduke::http;
namespace oauth = tenduke::oauth;
namespace rnd = tenduke::utl::random;
namespace xdtime = tenduke::time;

oauth::OAuthPKCEFlow::OAuthPKCEFlow(
        const std::shared_ptr<const oauth::OAuthConfiguration> config,
        const std::shared_ptr<const http::HTTPClient> httpClient,
        const std::shared_ptr<const tenduke::net::URLParser> urlParser,
        const std::shared_ptr<const tenduke::json::JSONParser> jsonParser,
        const std::shared_ptr<const tenduke::utl::Base64Encoder> base64Encoder,
        const std::shared_ptr<xdtime::Clock> clock,
        const std::shared_ptr<rnd::RandomUrlSafeString> randomUrlSafeString
) : stateLength(oauth::DEFAULT_STATE_LENGTH), pkceCodeVerifierLength(oauth::DEFAULT_CODEVERIFIER_LENGTH),
    config(config), httpClient(httpClient), urlParser(urlParser), jsonParser(jsonParser), base64Encoder(base64Encoder),
    clock(clock), randomUrlSafeString(randomUrlSafeString)
{}

std::unique_ptr<const oauth::OAuthRequest> oauth::OAuthPKCEFlow::authorize(
        const std::string &scopes,
        const std::map<std::string, std::string> &additionalParameters
)
{
    return std::unique_ptr<const oauth::OAuthRequest>(new oauth::OAuthPKCERequest(
        scopes,
        generateState(),
        additionalParameters,
        (config->usePKCE ? generateCodeVerifier() : ""),
        config,
        httpClient,
        urlParser,
        jsonParser,
        base64Encoder,
        clock
    ));
}

std::string oauth::OAuthPKCEFlow::generateState()
{
    return randomUrlSafeString->generate(stateLength);
}

std::string oauth::OAuthPKCEFlow::generateCodeVerifier()
{
    return randomUrlSafeString->generate(pkceCodeVerifierLength);
}
