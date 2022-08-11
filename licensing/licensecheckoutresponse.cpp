#include "./licensecheckoutresponse.h"

namespace licensing = tenduke::licensing;

licensing::LicenseCheckoutResponse::LicenseCheckoutResponse(
        const std::map<std::string, licensing::LicenseCheckoutItem> &items,
        const std::map<std::string, licensing::LicenseCheckoutError> &failedItems
)
 : failedItems(failedItems), items(items)
{
}

const std::map<std::string, licensing::LicenseCheckoutError> & licensing::LicenseCheckoutResponse::getFailedItems() const
{
    return failedItems;
}

const std::map<std::string, licensing::LicenseCheckoutItem> & licensing::LicenseCheckoutResponse::getItems() const
{
    return items;
}

bool licensing::LicenseCheckoutResponse::hasFailures() const
{
    return !failedItems.empty();
}

