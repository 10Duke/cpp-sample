#ifndef LICENSINGEXCEPTION_H
#define LICENSINGEXCEPTION_H

#include <stdexcept>

namespace tenduke { namespace licensing {


/** Generic licensing-related exception.
 *  Acts as base class for all more specific licensing-exceptions.
 *
 */
class LicensingException : public std::runtime_error
{
public:
    LicensingException(const std::string &message)
        : std::runtime_error(message)
    {}
};


}}

#endif // LICENSINGEXCEPTION_H
