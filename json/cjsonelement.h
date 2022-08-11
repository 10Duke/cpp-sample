#ifndef CJSONELEMENT_H
#define CJSONELEMENT_H

#include "./jsonelement.h"

#include <cjson/cJSON.h>

namespace tenduke { namespace json { namespace cjson {

class CjsonElement : virtual public tenduke::json::JSONElement
{
public:
    // Construction / destruction:
    CjsonElement(
            enum tenduke::json::JSONElement::Type type,
            cJSON *element,
            bool root
    ) : element(element), type(type), root(root)
    {}

    CjsonElement(
            enum tenduke::json::JSONElement::Type type,
            cJSON *element
            ) : CjsonElement(type, element, false)
    {}

    virtual ~CjsonElement();

    // Implementations:
    virtual std::string asString() const override;
    virtual enum Type getType() const override {return type;}

    // custom:
    void setRoot(bool p_root) {this->root = p_root;}

    // TBD: Should this be removed?
    cJSON * getCjsonElement() const {return element;}

protected:
    cJSON * element;

private:
    enum Type type;
    bool root;
};

}}}

#endif // CJSONELEMENT_H
