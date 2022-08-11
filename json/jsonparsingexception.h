#ifndef JSONPARSINGEXCEPTION_H
#define JSONPARSINGEXCEPTION_H

#include <stdexcept>

namespace tenduke { namespace json {

/** Thrown when JSON parsing failed.
 *  Most often when the data is not valid JSON.
 *
 */
class JSONParsingException : public std::runtime_error
{
public:
    JSONParsingException(const std::string& message)
        : std::runtime_error(message)
    {}
};

}}

#endif // JSONPARSINGEXCEPTION_H
