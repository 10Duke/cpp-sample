#include "licensinghttpstatuscodetoexception.h"

namespace xdhttp = tenduke::http;
namespace xdlicensing = tenduke::licensing;

void xdlicensing::LicensingHTTPStatusCodeToException::from403()
const
{
    // From some reason our application currently returns 403 when it should return 404.
    // NOTE: This loses the status code
    from404();
}
