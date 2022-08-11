#ifndef JSONPARSER_H
#define JSONPARSER_H

#include "./jsonelement.h"

#include <memory>
#include <string>

namespace tenduke { namespace json {

class JSONParser
{
public:
    virtual ~JSONParser() {}

    /** Parses JSON from given string.
     *
     *  @throws JSONParsingException when the JSON parsing failed for various reasons.
     */
    virtual std::unique_ptr<JSONElement> from(
            const char * const jsonAsString,
            size_t length
    ) const = 0;

    /** Parses JSON from given string.
     *
     *  @throws JSONParsingException when the JSON parsing failed for various reasons.
     */
    virtual std::unique_ptr<JSONElement> from(const std::string &string) const = 0;

};

}}

#endif // JSONPARSER_H
