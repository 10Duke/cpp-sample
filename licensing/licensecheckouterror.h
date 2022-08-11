#ifndef LICENSECHECKOUTERROR_H
#define LICENSECHECKOUTERROR_H

#include <string>

namespace tenduke { namespace licensing {


/** Describes an error for licensed item.
 *
 *  Checkout response can contain failures for some or all items.
 *  This object describes the error.
 */
class LicenseCheckoutError
{
public:
    LicenseCheckoutError() {}

    const std::string & getErrorCode() const {return errorCode;}
    const std::string & getErrorKey() const {return errorKey;}
    const std::string & getErrorMessage() const {return errorMessage;}
    const std::string & getErrorTechnical() const {return errorTechnical;}

    void setErrorCode(const std::string errorCode) {this->errorCode = errorCode;}
    void setErrorKey(const std::string errorKey) {this->errorKey = errorKey;}
    void setErrorMessage(const std::string errorMessage) {this->errorMessage = errorMessage;}
    void setErrorTechnical(const std::string errorTechnical) {this->errorTechnical = errorTechnical;}

private:
    std::string errorCode;
    std::string errorKey;
    std::string errorMessage;
    std::string errorTechnical;
};


}}

#endif // LICENSECHECKOUTERROR_H
