#ifndef LICENSECHECKOUTPARAMETERS_H
#define LICENSECHECKOUTPARAMETERS_H

#include "string"
#include "vector"

namespace tenduke { namespace licensing {


/** Parameters for license checkout request.
 *
 */
class LicenseCheckoutParameters
{
public:
    LicenseCheckoutParameters(
            const std::vector<std::string> items,
            const std::string hardwareId
    ) : items(items), hardwareId(hardwareId)
    {}
    virtual ~LicenseCheckoutParameters() {}

    const std::string & getHardwareId() const {return hardwareId;}
    const std::vector<std::string> & getItems() const {return items;}

private:
    const std::vector<std::string> items;
    const std::string hardwareId;
};


}}

#endif // LICENSECHECKOUTPARAMETERS_H
