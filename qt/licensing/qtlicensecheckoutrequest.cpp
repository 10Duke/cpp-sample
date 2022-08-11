#include "./qtlicensecheckoutrequest.h"

#include "licensing/licensecheckoutresponse.h"
#include "licensing/licensingexception.h"
#include "qt/utl/exceptiontostringlist.h"

namespace qtlic = tenduke::qt::licensing;
namespace xdlic = tenduke::licensing;


qtlic::QtLicenseCheckoutRequest::QtLicenseCheckoutRequest(std::unique_ptr<xdlic::LicenseCheckoutRequest> request)
    : request(std::move(request))
{
}


qtlic::QtLicenseCheckoutRequest::~QtLicenseCheckoutRequest()
{
}


void qtlic::QtLicenseCheckoutRequest::execute()
{
    std::unique_ptr<xdlic::LicenseCheckoutResponse> response;

    try {
        response = request->execute();
        Q_EMIT finished(*(response.get()));
    }
    catch (const std::exception &e) {
        Q_EMIT failed(std::current_exception());
    }
}
