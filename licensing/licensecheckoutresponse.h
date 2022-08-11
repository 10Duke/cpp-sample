#ifndef LICENSECHECKOUTRESPONSE_H
#define LICENSECHECKOUTRESPONSE_H

#include "./licensecheckouterror.h"
#include "./licensecheckoutitem.h"

#include <map>
#include <string>

namespace tenduke { namespace licensing {


/** Response from license checkout request.
 *
 *  Multiple licensed items can be checked out in single request. LicenseCheckoutResponse describes
 *  the results of the request, where multiple items may succeed or fail.
 *
 */
class LicenseCheckoutResponse
{
public:
    LicenseCheckoutResponse(
            const std::map<std::string, tenduke::licensing::LicenseCheckoutItem> &items,
            const std::map<std::string, tenduke::licensing::LicenseCheckoutError> &failedItems
    );

    /** Returns the items which failed.
     *
     *  @return the failed items. Key is the name of the licensed item, value is the error.
     */
    const std::map<std::string, tenduke::licensing::LicenseCheckoutError> & getFailedItems() const;

    /** Returns the items which were successfully checked out.
     *
     *  @return the successful items. Key is the name of the licensed item.
     */
    const std::map<std::string, tenduke::licensing::LicenseCheckoutItem> & getItems() const;

    /** Checks if any of the requested items failed to checkout successfully.
     *
     *  @return true if any of the requested items failed to checkout successfully
     */
    bool hasFailures() const;

private:
    const std::map<std::string, tenduke::licensing::LicenseCheckoutError> failedItems;
    const std::map<std::string, tenduke::licensing::LicenseCheckoutItem> items;
};


}}

#endif // LICENSECHECKOUTRESPONSE_H
