#ifndef JWTPARSINGEXCEPTION_H
#define JWTPARSINGEXCEPTION_H

#include <stdexcept>
#include <string>

namespace tenduke { namespace jwt {


/** Thrown when there is a problem parsing JWT.
 *
 */
class JWTParsingException : public std::runtime_error
{
public:
    JWTParsingException(const int code, const std::string &message)
        : std::runtime_error(message), code(code)
    {}

    int getCode() const {return this->code;}

private:
    const int code;
};


}}

#endif // JWTPARSINGEXCEPTION_H
