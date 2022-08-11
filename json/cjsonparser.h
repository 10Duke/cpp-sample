#ifndef CJSONPARSER_H
#define CJSONPARSER_H

#include "./jsonparser.h"

namespace tenduke { namespace json { namespace cjson {

class cJSONParser : public tenduke::json::JSONParser
{
public:
    cJSONParser();

    virtual std::unique_ptr<JSONElement> from(const std::string &string) const override;
    virtual std::unique_ptr<JSONElement> from(
            const char * const jsonAsString,
            size_t length
    ) const override;
};

}}}

#endif // CJSONPARSER_H
