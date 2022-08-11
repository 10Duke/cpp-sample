#ifndef LICENSECHECKOUTREQUEST_H
#define LICENSECHECKOUTREQUEST_H

#include "./licensecheckoutresponse.h"

#include <memory>

namespace tenduke { namespace licensing {


/** License checkout request.
 */
class LicenseCheckoutRequest
{
public:
    virtual ~LicenseCheckoutRequest() {}

    /** Execute the license checkout request synchronously.
     *
     *  @return the checkout response. IMPORTANT: Checkout failures (e.g. user does not have access to requested license)
     *          do not throw an exception, but are listed as failed items in the response.
     *
     *  @throws tenduke::http::HTTPException e.g. internal server errors, invalid configuration (e.g. HTTP 404 not found) et al.
     *  @throws tenduke::json::JSONParsingException JSON has errors (should not usually happen)
     *  @throws tenduke::licensing::LicensingRequestFailure e.g. invalid response (not JSON)
     *  @throws tenduke::net::NetworkingException e.g. timeouts, networking communication problems
     */
    virtual std::unique_ptr<LicenseCheckoutResponse> execute() = 0;
};


}}

#endif // LICENSECHECKOUTREQUEST_H
