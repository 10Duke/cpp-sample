#ifndef TENDUKE_OAUTH_OAUTHTOKENRESPONSE_H
#define TENDUKE_OAUTH_OAUTHTOKENRESPONSE_H

#include "json/jsonobject.h"

#include <string>

namespace tenduke { namespace oauth {


/** Encapsulates results from tenduke::oauth::OAuthTokenRequest.
 *
 */
class OAuthTokenResponse
{
public:
    OAuthTokenResponse(
            const std::string &accessToken,
            const std::string &refreshToken,
            const std::int64_t expiresAt,
            std::unique_ptr<tenduke::json::JSONObject> additionalProperties
    ) : accessToken(accessToken), refreshToken(refreshToken), expiresAt(expiresAt), additionalProperties(std::move(additionalProperties))
    {}

    const std::string accessToken;
    const std::string refreshToken;
    const std::int64_t expiresAt;
    std::unique_ptr<tenduke::json::JSONObject> additionalProperties;
};


}}

#endif // TENDUKE_OAUTH_OAUTHTOKENRESPONSE_H
