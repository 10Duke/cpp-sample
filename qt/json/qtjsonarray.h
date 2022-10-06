#ifndef QTJSONARRAY_H
#define QTJSONARRAY_H

#include "./qtjsonelement.h"
#include "json/jsonarray.h"

#include <QJsonArray>

namespace tenduke { namespace qt { namespace json {

class QtJSONArray: public tenduke::qt::json::QtJSONElement, public tenduke::json::JSONArray
{
public:
    QtJSONArray(
            QJsonArray &array,
            std::vector<std::shared_ptr<tenduke::json::JSONElement>> &items
    ) : tenduke::qt::json::QtJSONElement(tenduke::json::JSONElement::ARRAY), arrayElement(array), items(items)
    {}

    virtual ~QtJSONArray() override {}

public:
    // tenduke::json::JSONElement implementation
    virtual std::string asString() const override;

    // tenduke::json::JSONArray implementation
    virtual bool isEmpty() const override;
    virtual size_t getSize() const override;
    virtual const std::vector<std::shared_ptr<JSONElement>> & getValues() const override;

private:
    QJsonArray arrayElement;
    std::vector<std::shared_ptr<JSONElement>> items;
};

}}}

#endif // QTJSONARRAY_H
