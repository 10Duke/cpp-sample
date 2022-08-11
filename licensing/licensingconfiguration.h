#ifndef LICENSINGCONFIGURATION_H
#define LICENSINGCONFIGURATION_H

#include <string>

namespace tenduke { namespace licensing {


/** 10Duke licensing configuration.
 *
 */
class LicensingConfiguration
{
public:
    LicensingConfiguration() {}
    LicensingConfiguration(
            const std::string &accessToken,
            const std::string &endpoint
    ) : accessToken(accessToken), endpoint(endpoint)
    {}

    virtual ~LicensingConfiguration() {}

    const std::string & getAccessToken() const {return accessToken;}
    const std::string & getEndpoint() const {return endpoint;}

    /** Changes the access token, e.g. after OAuth-refresh.
     */
    void setAccessToken(const std::string &newAccessToken) {accessToken = newAccessToken;}

private:
    std::string accessToken;
    const std::string endpoint;
};


}}

#endif // LICENSINGCONFIGURATION_H
