#ifndef QTJSONPARSER_H
#define QTJSONPARSER_H

#include "json/jsonparser.h"

namespace tenduke { namespace qt { namespace json {


/** QT-implementation of tenduke::json::JSONParser.
 *
 */
class QtJSONParser : public tenduke::json::JSONParser
{
public:
    QtJSONParser() {}
    virtual ~QtJSONParser() override {}

    virtual std::unique_ptr<tenduke::json::JSONElement> from(const std::string &string) const override;
    virtual std::unique_ptr<tenduke::json::JSONElement> from(
            const char * const jsonAsString,
            size_t length
    ) const override;
};


}}}

#endif // QTJSONPARSER_H
