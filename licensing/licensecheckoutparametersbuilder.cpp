#include "./licensecheckoutparametersbuilder.h"

namespace licensing = tenduke::licensing;

licensing::LicenseCheckoutParametersBuilder::LicenseCheckoutParametersBuilder()
    : items(), hwId()
{
}

std::unique_ptr<const licensing::LicenseCheckoutParameters> licensing::LicenseCheckoutParametersBuilder::build()
{
    return std::unique_ptr<const licensing::LicenseCheckoutParameters>(new LicenseCheckoutParameters(
        items,
        hwId
    ));
}

licensing::LicenseCheckoutParametersBuilder& licensing::LicenseCheckoutParametersBuilder::hardwareId(const std::string hardwareId)
{
    hwId = hardwareId;
    return *this;
}

licensing::LicenseCheckoutParametersBuilder& licensing::LicenseCheckoutParametersBuilder::item(const std::string item)
{
    items.push_back(item);
    return *this;
}

licensing::LicenseCheckoutParametersBuilder& licensing::LicenseCheckoutParametersBuilder::withItems(const std::vector<std::string> &itemsToAdd)
{
    for(std::string item : itemsToAdd) {
        items.push_back(item);
    }

    return *this;
}
