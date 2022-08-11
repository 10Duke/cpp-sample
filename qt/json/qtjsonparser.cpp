#include "./qtjsonparser.h"
#include "./qtjsonarray.h"
#include "./qtjsonboolean.h"
#include "./qtjsonelement.h"
#include "./qtjsonnumber.h"
#include "./qtjsonobject.h"
#include "./qtjsonprimitive.h"
#include "./qtjsonstring.h"

#include "json/jsonelement.h"

#include <QByteArray>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>

namespace qtjson = tenduke::qt::json;
namespace xdjson = tenduke::json;


static qtjson::QtJSONElement * buildElement(QJsonValue value);


static qtjson::QtJSONArray * buildArray(QJsonArray array)
{
    std::vector<std::shared_ptr<xdjson::JSONElement>> items;
    int numItems = array.size();

    for (int i = 0, numItems = array.size(); i < numItems; i++) {
        items.push_back(std::shared_ptr<xdjson::JSONElement>(buildElement(array.at(i))));
    }

    return new qtjson::QtJSONArray(array, items);
}

static qtjson::QtJSONObject * buildObject(QJsonObject object)
{
    std::map<std::string, std::shared_ptr<tenduke::json::JSONElement>> properties;

    QJsonObject::const_iterator property = object.constBegin();
    while(property != object.constEnd()) {
        properties.emplace(
            property.key().toStdString(),
            std::shared_ptr<xdjson::JSONElement>(buildElement(property.value()))
        );
        property++;
    }

    return new qtjson::QtJSONObject(object, properties);
}

static qtjson::QtJSONBoolean * buildBoolean(QJsonValue value)
{
    return new qtjson::QtJSONBoolean(value, value.toBool());
}

static qtjson::QtJSONNumber * buildNumber(QJsonValue value)
{
    return new qtjson::QtJSONNumber(value, value.toDouble());
}

static qtjson::QtJSONString * buildString(QJsonValue value)
{
    return new qtjson::QtJSONString(value, value.toString().toStdString());
}

static qtjson::QtJSONElement * buildElement(QJsonValue value)
{
    if (value.isString()) {
        return buildString(value);
    }
    else if (value.isArray()) {
        return buildArray(value.toArray());
    }
    else if (value.isObject()) {
        return buildObject(value.toObject());
    }
    else if (value.isBool()) {
        return buildBoolean(value);
    }
    else if (value.isDouble()) {
        return buildNumber(value);
    }
    else if (value.isNull()) {
        return new qtjson::QtJSONPrimitive(xdjson::JSONElement::Type::NULLISH, value);
    }
    else {
        return new qtjson::QtJSONPrimitive(xdjson::JSONElement::Type::UNDEFINED, value);
    }
}


static qtjson::QtJSONElement * buildTree(QJsonDocument document)
{
    if (document.isNull()) {
        return new qtjson::QtJSONElement(xdjson::JSONElement::NULLISH);
    }
    else if (document.isObject()) {
        return buildObject(document.object());
    }
    else if (document.isArray()) {
        return buildArray(document.array());
    }
    else {
        return new qtjson::QtJSONElement(xdjson::JSONElement::UNDEFINED);
    }
}

std::unique_ptr<xdjson::JSONElement> qtjson::QtJSONParser::from(
        const char * const jsonAsString,
        size_t length
) const
{
    QByteArray bytes = QByteArray::fromRawData(jsonAsString, length);
    QJsonDocument document = QJsonDocument::fromJson(bytes);

    return std::unique_ptr<xdjson::JSONElement>(buildTree(document));
}

std::unique_ptr<xdjson::JSONElement> qtjson::QtJSONParser::from(const std::string &string) const
{
    return from(string.c_str(), string.size());
}

