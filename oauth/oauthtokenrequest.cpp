#include "./oauthtokenrequest.h"
#include "./oauthservererror.h"
#include "./oauthstate.h"

#include "http/httpexception.h"
#include "http/httprequestexception.h"
#include "json/jsonnumber.h"
#include "json/jsonparsingexception.h"
#include "json/jsonutils.h"

#include <cmath>

namespace http = tenduke::http;
namespace json = tenduke::json;
namespace oauth = tenduke::oauth;
namespace xdtime = tenduke::time;

oauth::OAuthTokenRequest::OAuthTokenRequest(
    std::unique_ptr<tenduke::http::HTTPCall> httpCall,
    std::shared_ptr<const tenduke::json::JSONParser> jsonParser,
    std::shared_ptr<tenduke::time::Clock> clock
) : httpCall(std::move(httpCall)), parseJSON (jsonParser), clock(clock), throwException(new http::HTTPStatusCodeToException())
{}


std::unique_ptr<oauth::OAuthTokenRequest> oauth::OAuthTokenRequest::create(
        const std::string &tokenEndpointUrl,
        const std::map<std::string, std::string> &parameters,
        const std::shared_ptr<const http::HTTPClient> httpClient,
        const std::shared_ptr<const json::JSONParser> jsonParser,
        const std::shared_ptr<xdtime::Clock> clock
) {
    http::HTTPRequestBuilder request = httpClient->request();

    request.post().url(tokenEndpointUrl)
            .contentType("application/x-www-form-urlencoded")
            .charset("UTF-8");

    for (auto const &parameter : parameters) {
        request.formParameter(parameter.first, parameter.second);
    }

    return std::unique_ptr<oauth::OAuthTokenRequest>(new oauth::OAuthTokenRequest(
        httpClient->call(request.build()),
        jsonParser,
        clock
    ));
}


std::unique_ptr<oauth::OAuthTokenResponse> oauth::OAuthTokenRequest::execute() const
{
    std::unique_ptr<http::HTTPResponse> response = httpCall->execute();

    if (! response->isSuccessful()) {
        // Handle errors

        // https://datatracker.ietf.org/doc/html/rfc6749
        // Section 5.2
        // The authorization server responds with an HTTP 400 (Bad Request)
        // status code (unless specified otherwise)...
        if (response->getStatusCode() == 400) {
            std::unique_ptr<const json::JSONObject> content = parseResponseBody(response->getPayloadAsString());

            throw oauth::OAuthServerError(
                oauth::OAuthException::Phase::TOKEN_REQUEST,
                json::getObjectPropertyAsString(content, "error"),
                json::getObjectPropertyAsString(content, "error_description")
            );
        }

        // Some other HTTP-status code
        throwException->fromHTTPStatus(response->getStatusCode());
    }

    return toResponse(response->getPayloadAsString());
}


std::unique_ptr<oauth::OAuthTokenResponse> oauth::OAuthTokenRequest::toResponse(const std::string &responsePayload) const
{
    std::unique_ptr<json::JSONObject> content = parseResponseBody(responsePayload);

    // Successful response should contain access token:
    // access_token
    std::shared_ptr<json::JSONElement> accessToken = content->removeProperty("access_token");
    if (accessToken == nullptr) {
        throw oauth::OAuthException(
            oauth::OAuthException::Phase::TOKEN_REQUEST,
            "missing_property_access_token",
            "Response does not have \"access_token\" property"
        );
    }

    // refresh_token
    std::shared_ptr<json::JSONElement> refreshToken = content->removeProperty("refresh_token");

    // expires_in
    int64_t expiresAt = oauth::TOKEN_DOES_NOT_EXPIRE;
    std::shared_ptr<json::JSONElement> expiresAtProperty = content->removeProperty("expires_in");
    if (expiresAtProperty != nullptr && expiresAtProperty->isNumber()) {
        std::int64_t expiresIn = std::round(dynamic_cast<json::JSONNumber *>(expiresAtProperty.get())->getValue());
        expiresAt = clock->epochSeconds() + expiresIn;
    }

    return std::unique_ptr<oauth::OAuthTokenResponse>(new oauth::OAuthTokenResponse(
        accessToken->asString(),
        (refreshToken == nullptr ? "" : refreshToken->asString()),
        expiresAt,
        std::move(content)
    ));
}


std::unique_ptr<json::JSONObject> oauth::OAuthTokenRequest::parseResponseBody(const std::string &responsePayload) const
{
    std::unique_ptr<json::JSONElement> content = parseJSON->from(responsePayload);

    if (! content->isObject()) {
        std::throw_with_nested(oauth::OAuthException(
            oauth::OAuthException::TOKEN_REQUEST,
            "invalid_token_response",
            "The response payload is not JSON object"
        ));
    }

    return std::unique_ptr<json::JSONObject>(dynamic_cast<json::JSONObject *>(content.release()));
}
