#ifndef OAUTHCONFIGURATION_H
#define OAUTHCONFIGURATION_H

#include "./oauthbackendconfiguration.h"
#include "./oauthclientconfiguration.h"

#include <memory>
#include <string>

namespace tenduke { namespace oauth {


/** Container for OAuth-configuration.
 *
 */
class OAuthConfiguration : public OAuthBackendConfiguration, public OAuthClientConfiguration
{
public:
    OAuthConfiguration(
            const std::string authorizationEndpointUrl,
            const std::string tokenEndpointUrl,
            const std::string clientId,
            const std::string clientSecret,
            const std::string redirectURI,
            bool usePKCE
    ) : OAuthBackendConfiguration(authorizationEndpointUrl, tokenEndpointUrl),
        OAuthClientConfiguration(clientId, clientSecret, redirectURI, usePKCE)
    {}

    OAuthConfiguration(
            const OAuthBackendConfiguration &backendConfiguration,
            const OAuthClientConfiguration &clientConfiguration
    ) : OAuthBackendConfiguration(backendConfiguration),
        OAuthClientConfiguration(clientConfiguration)
    {}

/*
    const std::string authorizationEndpointUrl;
    const std::string tokenEndpointUrl;

    const std::string clientId;
    const std::string clientSecret;
    const std::string redirectURI;

    const bool usePKCE;
*/
};


}}

#endif // OAUTHCONFIGURATION_H
