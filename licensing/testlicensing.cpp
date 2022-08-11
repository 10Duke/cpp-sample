#include "testlicensing.h"

#include "./defaultlicensingclient.h"
#include "./licensecheckoutparametersbuilder.h"
#include "http/libcurlhttpclient.h"
#include "json/cjsonparser.h"
#include "net/libcurlurlencoder.h"

#include <iostream>

namespace licensing = tenduke::licensing;
namespace json = tenduke::json;
namespace http = tenduke::http;
namespace net = tenduke::net;
namespace netcurl = tenduke::net::curl;

void licensing::TestLicensing::execute()
{
    std::shared_ptr<json::JSONParser> jsonParser(new tenduke::json::cjson::cJSONParser());
    std::shared_ptr<http::HTTPClient> httpClient(new tenduke::http::curl::LibCurlHTTPClient(
        std::shared_ptr<net::URLEncoder>(new netcurl::LibCurlURLEncoder()),
        5000L,
        true
    ));
    std::shared_ptr<licensing::LicensingConfiguration> config(new licensing::LicensingConfiguration(
        "iqucsbdkgki4uaj22vqjbepufr",
// Expired token:
//      "a7rj9bcstefl9e9366dlsfiak2",

// Valid URL:
        "https://genco.10duke.com/authz/"
// Malformed URL:
//        "https://genco.10duke.com:-80/authz/"
// Unknown host:
//        "https://gencozor.10duke.com/authz/"
// Connection time out:
//        "https://genco.10duke.com:6969/authz/"
// Not found (invalid configuration):
//        "https://genco.10duke.com/no-such-endpoint"
    ));

    licensing::DefaultLicensingClient licenses(
                config,
                httpClient,
                jsonParser
    );

    std::unique_ptr<licensing::LicenseCheckoutRequest> request = licenses.checkout(
        licensing::LicenseCheckoutParametersBuilder()
            .hardwareId("veijo")
            .item("qt-demo-1")
            .build()
    );

    try {
        std::unique_ptr<licensing::LicenseCheckoutResponse> response = request->execute();

        std::cout << "Successfully checked out:\n";
        for (auto const &item : response->getItems()) {
            std::cout << "- Item " << item.first << "\n";
        }

        if (response->hasFailures()) {
            std::cout << "License checkout failures:\n";
            for (auto const &item : response->getFailedItems()) {
                std::cout << "Item " << item.first << ": " << item.second.getErrorMessage() << "\n";
            }
        }

    } catch (std::exception &e) {
        std::cout << e.what() << "\n";
    }
}
