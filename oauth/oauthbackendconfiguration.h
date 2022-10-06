#ifndef OAUTHBACKENDCONFIGURATION_H
#define OAUTHBACKENDCONFIGURATION_H

#include "crypto/publickey.h"

#include <memory>
#include <string>

namespace tenduke { namespace oauth {


/** Container for OAuth-backend configuration.
 *
 */
class OAuthBackendConfiguration
{
public:
    OAuthBackendConfiguration(
            const std::string &authorizationEndpointUrl,
            const std::string &tokenEndpointUrl
    ) : authorizationEndpointUrl(authorizationEndpointUrl), tokenEndpointUrl(tokenEndpointUrl)
    {}

    const std::string authorizationEndpointUrl;
    const std::string tokenEndpointUrl;
};


}}

#endif // OAUTHBACKENDCONFIGURATION_H
