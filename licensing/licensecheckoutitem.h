#ifndef LICENSECHECKOUTITEM_H
#define LICENSECHECKOUTITEM_H

#include <string>

namespace tenduke { namespace licensing {


/** Describes licensed item successfully checked out.
 *
 */
class LicenseCheckoutItem
{
public:
    LicenseCheckoutItem() : LicenseCheckoutItem("") {}
    LicenseCheckoutItem(const std::string &name) : name(name) {}

    const std::string & getName() const {return name;}

private:
    const std::string name;
};


}}

#endif // LICENSECHECKOUTITEM_H
