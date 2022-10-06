#include "testlicensing.h"

#include "./defaultlicensingclient.h"
#include "./licensecheckoutparametersbuilder.h"
#include "crypto/rsapublickeyfrompemstring.h"
#include "http/libcurlhttpclient.h"
#include "json/cjsonparser.h"
#include "jwt/createjwtparser.h"
#include "net/libcurlurlencoder.h"
#include "utl/defaultbase64decoder.h"

#include <iostream>

namespace libcrypto = tenduke::crypto::libcrypto;
namespace licensing = tenduke::licensing;
namespace json = tenduke::json;
namespace http = tenduke::http;
namespace net = tenduke::net;
namespace netcurl = tenduke::net::curl;
namespace xdcrypto = tenduke::crypto;
namespace xdjwt = tenduke::jwt;
namespace xdutl = tenduke::utl;

static std::string ID_TOKEN_VERIFICATION_KEY_RSA_PEM =
        "-----BEGIN PUBLIC KEY-----\n"
        "MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEAywVSSuHKmyNrcT8JArxo\n"
        "IqTuWdCvG2R78p1Osdav8ivjQWqDnjR37tt7L+U+sopV4ka4gUQVi7Ie87l2cJwh\n"
        "sJ6uAQWfp6K7r+H/yH+ak+F8EvcWLFNqRjbvgAu0MqSt16bkZX01AanBca3yioZ+\n"
        "Ihe7DryKSbR1n8IMU7DRUiZzB4c9qdPphmDwxzryaiTkE1QJyXGjpSdvwwIdXE9u\n"
        "XE12zSeR2+CRKWTPZsnRBKpSDdrEwE8nSRW5XmDppnpoAvb6YI7ULtXZN354atbH\n"
        "sC1s+siHsjD7zB//cUzsRtge4YCTOoIs4thirizP3uXg8xJSs1Quie1GvZt0ufwl\n"
        "jMQnbBR7Le1ctV7sCZFom4XJGewGpnXQP9TBBpofH1RhjmBBRyruvbX3xGj2mKpi\n"
        "hy6k3FzoxZ580Pv1KGo1CYjLgfXFSmwnq/MJ6bE1wR9rmexOE1b2laWsTbTdpZB4\n"
        "/3mHGQ1yd5w+7ZjOQ1/K0g5FHm5yKK9cJSvQihN/BpGN5YhvwkpjhAhJlF+csLg4\n"
        "DGXl5GxnTfP1ZSUywOP2Da4PzpaghsDJpkBkh6rKDK+mJ9v0He1BfvhxIqAjVnur\n"
        "IRZriZ6mXwTM7C9v30IBIgnadgLkyptuj+/1F3Z3m2+Ix6uLpZGUQpWVgMcC2uM5\n"
        "kcU6rWjrfEfAVM23axgy7c0CAwEAAQ==\n"
        "-----END PUBLIC KEY-----"
;


static std::unique_ptr<const xdcrypto::PublicKey> createPublicKey(std::string key)
{
    if (key.empty()) {
        return std::unique_ptr<const xdcrypto::PublicKey>();
    }

    // NOTE: This will throw CryptoException if the key is invalid.
    return libcrypto::RSAPublicKeyFromPEMString().from(key);
}

void licensing::TestLicensing::execute()
{
    std::shared_ptr<json::JSONParser> jsonParser(new tenduke::json::cjson::cJSONParser());
    std::shared_ptr<http::HTTPClient> httpClient(new tenduke::http::curl::LibCurlHTTPClient(
        std::shared_ptr<net::URLEncoder>(new netcurl::LibCurlURLEncoder()),
        5000L,
        true
    ));
    std::shared_ptr<xdutl::Base64Decoder> base64Decoder (new xdutl::DefaultBase64Decoder());
    std::shared_ptr<const xdjwt::JWTParser> jwtParser = xdjwt::createJWTParser(
        "sha256",
        createPublicKey(ID_TOKEN_VERIFICATION_KEY_RSA_PEM),
        base64Decoder,
        jsonParser
    );
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
        jwtParser
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
