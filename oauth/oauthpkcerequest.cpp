#include "./oauthpkcerequest.h"

#include "./oauthstateimpl.h"
#include "./oauthexception.h"
#include "./oauthservererror.h"

#include "crypto/sha256utl.h"
#include "http/urlbuilder.h"
#include "json/jsonobject.h"
#include "json/jsonnumber.h"
#include "net/malformedurlexception.h"

#include <cmath>

namespace crypto = tenduke::crypto;
namespace http = tenduke::http;
namespace json = tenduke::json;
namespace net = tenduke::net;
namespace oauth = tenduke::oauth;
namespace xdtime = tenduke::time;

oauth::OAuthPKCERequest::OAuthPKCERequest(
        const std::string scopes,
        const std::string state,
        const std::map<std::string, std::string> additionalParameters,
        const std::string codeVerifier,
        const std::shared_ptr<const oauth::OAuthConfiguration> config,
        const std::shared_ptr<const http::HTTPClient> httpClient,
        const std::shared_ptr<const tenduke::net::URLParser> urlParser,
        const std::shared_ptr<const tenduke::json::JSONParser> jsonParser,
        const std::shared_ptr<const tenduke::utl::Base64Encoder> base64Encoder,
        const std::shared_ptr<xdtime::Clock> clock
) : scopes(scopes), state(state), additionalParameters(additionalParameters), codeVerifier(codeVerifier),
    config(config), http(httpClient), parseURL(urlParser), parseJSON(jsonParser), base64Encoder(base64Encoder), clock(clock)
{}


/** Returns first parameter of parameterName. If not found, returns empty string.
 */
static std::string getFirst(const std::string &parameterName, const std::multimap<std::string, std::string> &map)
{
    std::map<std::string, std::string>::const_iterator iterator = map.find(parameterName);
    if (iterator == map.end()) {
        return "";
    }
    return iterator->second;
}

std::unique_ptr<oauth::OAuthState> oauth::OAuthPKCERequest::handleCallback(const std::string &callbackUrl)
const
{
    std::unique_ptr<net::URL> url;

    // Might throw tenduke::net::URLException
    // Might throw tenduke::net::MalformedURLException
    url = parseURL->from(callbackUrl);

    std::multimap<std::string, std::string> queryParameters = url->getQueryParameters();

    if(queryParameters.count("error") > 0) {
        throw oauth::OAuthServerError(
            oauth::OAuthException::Phase::AUTHORIZATION,
            getFirst("error", queryParameters),
            getFirst("error_description", queryParameters)
        );
    }

    return this->handleSuccessfulCallback(queryParameters);
}


std::unique_ptr<oauth::OAuthState> oauth::OAuthPKCERequest::handleSuccessfulCallback(const std::multimap<std::string, std::string> &queryParameters)
const
{
    // Check presence of "state"
    std::map<std::string, std::string>::const_iterator iterator = queryParameters.find("state");
    if (iterator == queryParameters.end()) {
        throw oauth::OAuthException(
                    oauth::OAuthException::Phase::AUTHORIZATION,
                    "invalid_callback",
                    "Authorization callback does not contain \"state\""
        );
    }

    // Validate "state"
    if (iterator->second != this->state) {
        throw oauth::OAuthException(
                    oauth::OAuthException::Phase::AUTHORIZATION,
                    "invalid_state",
                    "State does not match!"
        );
    }

    iterator = queryParameters.find("code");
    if (iterator == queryParameters.end()) {
        throw oauth::OAuthException(
                    oauth::OAuthException::Phase::AUTHORIZATION,
                    "invalid_callback",
                    "Authorization callback does not contain \"code\""
        );
    }

    return handleTokenResponse(createTokenRequest(iterator->second)->execute());
}


std::unique_ptr<oauth::OAuthTokenRequest> oauth::OAuthPKCERequest::createTokenRequest(const std::string &code) const
{
    return oauth::OAuthTokenRequest::create(
        config->tokenEndpointUrl,
        buildTokenRequestParameters(code),
        http,
        parseJSON,
        clock
    );
}


std::map<std::string, std::string> oauth::OAuthPKCERequest::buildTokenRequestParameters(const std::string &code)
const
{
    std::map<std::string, std::string> parameters;

    parameters["grant_type"] = "authorization_code";
    parameters["client_id"] = config->clientId;
    parameters["redirect_uri"] = config->redirectURI;
    parameters["code"] = code;

    if (isPKCERequest()) {
        parameters["code_verifier"] = codeVerifier;
    }

    if (!config->clientSecret.empty()) {
        parameters["client_secret"] = config->clientSecret;
    }

    return parameters;
}


static std::map<std::string, std::string> toMap(const json::JSONObject * object)
{
    std::map<std::string, std::string> map;
    for (auto const &property : object->getProperties()) {
        map.emplace(property.first, property.second->asString());
    }
    return map;
}


std::unique_ptr<oauth::OAuthState> oauth::OAuthPKCERequest::handleTokenResponse(std::unique_ptr<oauth::OAuthTokenResponse> response)
const
{
    // TBD: Should we return stuff as JSONElements instead?
    std::map<std::string, std::string> additionalProperties = toMap(response->additionalProperties.get());

    return std::unique_ptr<oauth::OAuthState>(new oauth::OAuthStateImpl(
                response->accessToken,
                response->refreshToken,
                response->expiresAt,
                additionalProperties,
                config,
                http,
                parseJSON,
                clock
    ));
}


bool oauth::OAuthPKCERequest::isPKCERequest()
const
{
    return ! codeVerifier.empty();
}


std::string oauth::OAuthPKCERequest::buildAuthorizationRequestUrl()
const
{
    http::URLBuilder builda = http->request().url();

    return builda.baseURL(config->authorizationEndpointUrl)
          .queryParameters(buildAuthorizationRequestParameters())
          .buildString();
}


std::map<std::string, std::string> oauth::OAuthPKCERequest::buildAuthorizationRequestParameters()
const
{
  std::map<std::string, std::string> parameters;

  parameters["response_type"] = "code";
  parameters["scope"] = scopes;
  parameters["redirect_uri"] = config.get()->redirectURI;
  parameters["client_id"] = config.get()->clientId;
  parameters["state"] = this->state;

  if (isPKCERequest()) {
      createCodeChallenge(parameters);
  }

  for (auto const& additionalParameter : additionalParameters) {
     parameters[additionalParameter.first] = additionalParameter.second;
  }

  return parameters;
}


void oauth::OAuthPKCERequest::createCodeChallenge(std::map<std::string, std::string> &parameters)
const
{
    unsigned char hash [crypto::SHA256_HASH_LENGTH_BYTES];

    crypto::calculateSHA256(
                (const unsigned char *) codeVerifier.c_str(),
                codeVerifier.size(),
                hash
    );

    parameters["code_challenge"] = base64Encoder->encodeUrlSafe(hash, sizeof(hash));
    parameters["code_challenge_method"] = "S256";
}
