#ifndef LICENSINGREQUESTFAILURE_H
#define LICENSINGREQUESTFAILURE_H

#include "./licensingexception.h"

namespace tenduke { namespace licensing {


/** Thrown when there is some licensing-related error.
 *
 */
class LicensingRequestFailure : public tenduke::licensing::LicensingException
{
public:
    LicensingRequestFailure(const std::string &message)
        : tenduke::licensing::LicensingException(message)
    {}
};


}}

#endif // LICENSINGREQUESTFAILURE_H
