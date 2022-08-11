#ifndef OAUTHCONFIGURATION_H
#define OAUTHCONFIGURATION_H

#include "crypto/publickey.h"

#include <memory>
#include <string>

namespace tenduke { namespace oauth {


/** Container for OAuth-configuration.
 *
 */
class OAuthConfiguration
{
public:
    OAuthConfiguration(
            const std::string authorizationEndpointUrl,
            const std::string tokenEndpointUrl,
            const std::string clientId,
            const std::string clientSecret,
            const std::string redirectURI,
            bool usePKCE
    ) : authorizationEndpointUrl(authorizationEndpointUrl), tokenEndpointUrl(tokenEndpointUrl),
        clientId(clientId), clientSecret(clientSecret), redirectURI(redirectURI), usePKCE(usePKCE)
    {}

    const std::string authorizationEndpointUrl;
    const std::string tokenEndpointUrl;

    const std::string clientId;
    const std::string clientSecret;
    const std::string redirectURI;

    const bool usePKCE;
};


}}

#endif // OAUTHCONFIGURATION_H
