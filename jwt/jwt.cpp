#include "jwt.h"

namespace jwt = tenduke::jwt;

jwt::JWT::JWT(std::map<std::string, std::string> claims)
    : claims (claims)
{
}

bool jwt::JWT::hasClaim(const std::string &name)
{
    return claims.count(name) > 0;
}

const std::string * jwt::JWT::claim(const std::string &name)
{
    std::map<std::string, std::string>::iterator iterator = claims.find(name);

    if (iterator == claims.end()) {
        return nullptr;
    }

    return &(iterator->second);
}

