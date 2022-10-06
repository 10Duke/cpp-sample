#ifndef OIDCCONFIGURATION_H
#define OIDCCONFIGURATION_H

#include "oauth/oauthconfiguration.h"

#include <memory>
#include <string>

namespace tenduke { namespace oauth { namespace oidc {


/** OIDC-configuration.
 *
 */
class OIDCConfiguration
{
public:
    /** Constructs new instance.
     *
     *  @param issuer expected issuer (from the OIDC provider)
     *  @param verificationKey key to use to verify the ID-token signature
     */
    OIDCConfiguration(
            const std::string &issuer,
            std::shared_ptr<const tenduke::crypto::PublicKey> verificationKey,
            const std::string &userinfoEndpoint
    ) : issuer(issuer), verificationKey(verificationKey), algorithm("sha256"), userinfoEndpoint(userinfoEndpoint)
    {}

    const std::string issuer;
    const std::shared_ptr<const tenduke::crypto::PublicKey> verificationKey;
    const std::string algorithm;
    const std::string userinfoEndpoint;
};


}}}

#endif // OIDCCONFIGURATION_H
