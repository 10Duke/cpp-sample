#ifndef JWT_H
#define JWT_H

#include <map>
#include <string>

namespace tenduke { namespace jwt {


/** Very simple JWT implementation.
 *
 *  This will likely be heavily modified in the future.
 */
class JWT
{
public:
    JWT(std::map<std::string, std::string> claims);

    bool hasClaim(const std::string &name);
    const std::string * claim(const std::string &name);
    const std::map<std::string, std::string> & getClaims() {return claims;}

private:
    std::map<std::string, std::string> claims;
};


}}

#endif // JWT_H
