#ifndef LICENSECHECKOUTPARAMETERSBUILDER_H
#define LICENSECHECKOUTPARAMETERSBUILDER_H

#include "./licensecheckoutparameters.h"

#include <memory>
#include <string>
#include <vector>

namespace tenduke { namespace licensing {


/** Builder for license checkout request parameters.
 *
 */
class LicenseCheckoutParametersBuilder
{
public:
    LicenseCheckoutParametersBuilder();

    /** Builds the request parameters object.
     *
     *  @return the request parameters object
     */
    std::unique_ptr<const LicenseCheckoutParameters> build();

    /** Sets the hardware id.
     *
     *  @param harwareId hardware identifier
     *  @return this builder
     */
    LicenseCheckoutParametersBuilder& hardwareId(std::string hardwareId);

    /** Adds single item to be checked out.
     *
     *  @param item item name
     *  @return this builder
     */
    LicenseCheckoutParametersBuilder& item(std::string item);

    /** Adds multiple items to be checked out
     *
     *  @param itemsToAdd names of the items
     *  @return this builder
     */
    LicenseCheckoutParametersBuilder& withItems(const std::vector<std::string> &itemsToAdd);

private:
    std::vector<std::string> items;
    std::string hwId;
};


}}

#endif // LICENSECHECKOUTPARAMETERSBUILDER_H
