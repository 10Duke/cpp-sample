#ifndef NETWORKINGEXCEPTION_H
#define NETWORKINGEXCEPTION_H

#include <stdexcept>

namespace tenduke { namespace net {


/** Generic networking exception.
 *  Base class to several more specific exceptions.
 */
class NetworkingException : public std::runtime_error
{
public:
    NetworkingException(const std::string& message)
        : std::runtime_error(message)
    {}
};


}}

#endif // NETWORKINGEXCEPTION_H
