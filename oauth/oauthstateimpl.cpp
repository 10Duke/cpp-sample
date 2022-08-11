#include "./oauthstateimpl.h"

#include "./oauthexception.h"

#include "json/jsonnumber.h"
#include "json/jsonutils.h"

#include <cinttypes>
#include <cmath>


namespace json = tenduke::json;
namespace oauth = tenduke::oauth;

const std::string * oauth::OAuthStateImpl::getAdditionalProperty(const std::string &name) const
{
    std::map<std::string, std::string>::const_iterator iterator = additionalProperties.find(name);

    if (iterator == additionalProperties.end()) {
        return nullptr;
    }

    return &(iterator->second);
}


bool oauth::OAuthStateImpl::refresh()
{
    if (refreshToken.empty()) {
        return false;
    }

    std::unique_ptr<oauth::OAuthTokenResponse> response = createRefreshTokenRequest()->execute();
    this->accessToken = response->accessToken;
    this->expiresAt = response->expiresAt;
    this->refreshToken = response->refreshToken;

    return true;
}


std::unique_ptr<oauth::OAuthTokenRequest> oauth::OAuthStateImpl::createRefreshTokenRequest()
const
{
    return oauth::OAuthTokenRequest::create(
        config->tokenEndpointUrl,
        buildRefreshRequestParameters(),
        http,
        parseJSON,
        clock
    );
}


std::map<std::string, std::string> oauth::OAuthStateImpl::buildRefreshRequestParameters()
const
{
    std::map<std::string, std::string> parameters;

    parameters["grant_type"] = "refresh_token";
    parameters["client_id"] = config->clientId;
    parameters["client_secret"] = config->clientSecret;
    parameters["refresh_token"] = refreshToken;

    return parameters;
}

