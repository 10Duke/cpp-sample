#ifndef OAUTHCLIENTCONFIGURATION_H
#define OAUTHCLIENTCONFIGURATION_H

#include "crypto/publickey.h"

#include <memory>
#include <string>

namespace tenduke { namespace oauth {


/** Container for OAuth-client-configuration.
 *
 */
class OAuthClientConfiguration
{
public:
    OAuthClientConfiguration(
            const std::string clientId,
            const std::string clientSecret,
            const std::string redirectURI,
            bool usePKCE
    ) : clientId(clientId), clientSecret(clientSecret), redirectURI(redirectURI), usePKCE(usePKCE)
    {}

    const std::string clientId;
    const std::string clientSecret;
    const std::string redirectURI;

    const bool usePKCE;
};


}}

#endif // OAUTHCLIENTCONFIGURATION_H
