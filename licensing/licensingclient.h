#ifndef LICENSING_CLIENT_H
#define LICENSING_CLIENT_H

#include "./licensecheckoutparameters.h"
#include "./licensecheckoutrequest.h"

#include <memory>

namespace tenduke { namespace licensing {


/** Client for license consumption operations.
 *
 */
class LicensingClient
{
public:
    virtual ~LicensingClient() {}

    /** Creates a tenduke::licensing::LicenseCheckoutRequest.
     *
     *  @param request the request parameters
     *  @return the request
     */
    virtual std::unique_ptr<tenduke::licensing::LicenseCheckoutRequest> checkout(std::unique_ptr<const tenduke::licensing::LicenseCheckoutParameters> request) = 0;
};


}}

#endif // LICENSING_CLIENT_H
