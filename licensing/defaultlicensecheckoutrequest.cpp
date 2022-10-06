#include "./defaultlicensecheckoutrequest.h"
#include "./licensecheckoutitem.h"
#include "./licensingrequestfailure.h"
#include "./licensinghttpstatuscodetoexception.h"
#include "http/httpexception.h"
#include "http/httprequestbuilder.h"
#include "http/httprequestexception.h"
#include "http/notfound.h"
#include "http/urlbuilder.h"
#include "json/jsonboolean.h"
#include "json/jsonobject.h"
#include "json/jsonparsingexception.h"
#include "json/jsonutils.h"


namespace licensing = tenduke::licensing;
namespace http = tenduke::http;
namespace json = tenduke::json;


licensing::DefaultLicenseCheckoutRequest::DefaultLicenseCheckoutRequest(
    std::shared_ptr<const jwt::JWTParser> jwtParser,
    std::shared_ptr<const http::HTTPClient> httpClient,
    std::shared_ptr<const licensing::LicensingConfiguration> config,
    std::unique_ptr<const licensing::LicenseCheckoutParameters> params
) : parseJWT(jwtParser), http(httpClient), config(config), params(std::move(params)),
    throwException(new licensing::LicensingHTTPStatusCodeToException())
{
}


licensing::DefaultLicenseCheckoutRequest::~DefaultLicenseCheckoutRequest()
{
}


std::unique_ptr<licensing::LicenseCheckoutResponse> licensing::DefaultLicenseCheckoutRequest::execute()
{
    // Build the HTTP-request
    std::string authorizationHeader = "Bearer ";
    authorizationHeader += config->getAccessToken();

    http::HTTPRequestBuilder request = http->request();
    http::URLBuilder urlBuilder = request.url();

    // We want the response as JWT:
    urlBuilder.baseURL(config->getEndpoint() + ".jwt");

    if (! (params->getHardwareId().empty())) {
        urlBuilder.queryParameter("hw", params->getHardwareId());
    }
    for (auto const& item : params->getItems()) {
        urlBuilder.queryParameter(item);
    }

    std::string url = urlBuilder.buildString();

    // Execute the request:
    request.get()
            .url(url)
            .header("Authorization", authorizationHeader);

    auto httpCall = http->call(request.build());
    auto httpResponse = httpCall->execute();

    // Note: The current /authz API respond with following error codes:
    if (! httpResponse->isSuccessful()) {
        throwException->fromHTTPStatus(httpResponse->getStatusCode());
    }

    return parseResponsePayload(httpResponse->getPayloadAsString());
}


std::unique_ptr<licensing::LicenseCheckoutResponse> licensing::DefaultLicenseCheckoutRequest::parseResponsePayload(const std::string &payload)
{
    tenduke::jwt::JWT jwt = parseJWT->from(payload);

    //std::unique_ptr<json::JSONElement> response = parseResponse->from(payload.c_str());

    // Expect JSON object response:
    //if (! response->isObject()) {
    //    throw licensing::LicensingRequestFailure("Invalid response, expected JSON object");
    //}

    std::map<std::string, std::string> itemNameLookup;

    // Build lookup structure:
    // item prefixes: qt-demo-item-1 --> qt-demo-item-1_
    for (auto const &item : params->getItems()) {
        itemNameLookup.emplace(item, item + '_');
    }

    std::map<std::string, licensing::LicenseCheckoutItem> items;
    std::map<std::string, licensing::LicenseCheckoutError> failedItems;

    // Loop through the JSON properties:
    for (auto const &property : jwt.getClaims()) {
        const std::string &key = property.first;
        const std::string &value = property.second;

        // Check for licensed item with exact match.
        // Example: "qt-demo-item-1": true
        if (itemNameLookup.count(key) > 0) {
            // Successfully checked out
            if (value == "true") {
                items.emplace(key, licensing::LicenseCheckoutItem(key));
            }
            // ERROR: Unexpected response, assume not checked out
            else {
                licensing::LicenseCheckoutError &failedItem = failedItems[key];
                failedItem.setErrorCode("checkoutFailed");
                failedItem.setErrorKey("checkoutFailed");
                failedItem.setErrorMessage("Checkout failed: Unexpected response");
                failedItem.setErrorTechnical("Expected boolean true, got something else");
            }
        }
        // Check for errors, use prefixes:
        else {
            for (auto const &itemNamePair: itemNameLookup) {
                const std::string &prefix = itemNamePair.second;

                // Found a match, check for errors:
                if (key.find(prefix) == 0) {
                    std::string itemName = itemNamePair.first;
                    licensing::LicenseCheckoutError &failedItem = failedItems[itemName];

                    std::string property = key;
                    property.erase(0, prefix.size());

                    if (property == "errorCode") {
                        failedItem.setErrorCode(value);
                    } else if (property == "errorKey") {
                        failedItem.setErrorKey(value);
                    } else if (property == "errorMessage") {
                        failedItem.setErrorMessage(value);
                    } else if (property == "errorTechnical") {
                        failedItem.setErrorTechnical(value);
                    } else {
                        // Ignored intentionally
                    }
                }
            }
        }
    }

    return std::unique_ptr<licensing::LicenseCheckoutResponse>(new licensing::LicenseCheckoutResponse(items, failedItems));
}

// Some sample responses:
/*
  "qt-demo-item-1_errorKey":"notAuthorized",
  "iss":"e0fb530b-27a3-4bf5-9371-5e67d9691590",
  "qt-demo-item-1_errorCode":"notAuthorized",
  "qt-demo-item-1_errorTechnical":"Authorization failed for \"qt-demo-item-1\"",
  "qt-demo-item-1_errorMessage":"Access to \"qt-demo-item-1\" denied",
  "iat":1648556679
}
{
    "demo-1":true
  , "lic":"09e31ad4-a4fb-4274-9986-0c4b5b2a78ef"
  , "iss":"e0fb530b-27a3-4bf5-9371-5e67d9691590"
  , "ibb":1648166400
  , "exp":1648628710
  , "iat":1648628530
  , "jti":"c6485483-23af-4486-975a-3a8631eccf4f"
  , "hw":"simulated-hardware-id"
  , "rfr":1648628665
}

eyJhbGciOiJSUzI1NiJ9.eyJxdC1kZW1vLWl0ZW0tMV9lcnJvcktleSI6Im5vdEF1dGhvcml6ZWQiLCJpc3MiOiJlMGZiNTMwYi0yN2EzLTRiZjUtOTM3MS01ZTY3ZDk2OTE1OTAiLCJxdC1kZW1vLWl0ZW0tMV9lcnJvckNvZGUiOiJub3RBdXRob3JpemVkIiwicXQtZGVtby1pdGVtLTFfZXJyb3JUZWNobmljYWwiOiJBdXRob3JpemF0aW9uIGZhaWxlZCBmb3IgXCJxdC1kZW1vLWl0ZW0tMVwiIiwicXQtZGVtby1pdGVtLTFfZXJyb3JNZXNzYWdlIjoiQWNjZXNzIHRvIFwicXQtZGVtby1pdGVtLTFcIiBkZW5pZWQiLCJleHAiOjE2NjQ4ODgwOTQsImlhdCI6MTY2NDgwMTY5NH0.GOCC-VS3l7hvOPnTK3bMJpUcpoMtu6abBYn_1rFSIdrTr7r2Z_plkfyLeU_JD0HPudDcY6uDswW6UNIvTOLI-D3RcdzhQWoRLdXsgeqMkJVjlxhA8--eFa-tpphV6x3L3r4brTzz6imVeNAi326m2o3IPfx_NqiXJSuZkyzpwY7vIzewx71iBr-ahcfFMc_Ttc32h-YWr3TqKDo2BPqso0XfL8zJal2QF9SClyNECpmGE2fTHB_19BaaAfa7i_LY2ijyNr1FZ6rBr1lpbJHex9MXDoV9ZB2UIvRhMubH5f25ctiq2TZFDX_6cWTIyBTHdJhDiDUBIo4iv1KaKKgADpPErl_nFSASt76xqdB3Aes8veL_x-bprfjZoG-ZX64cs5giSejNpesLxoe196giY9XW1WZ7qN75R7UZlJ7iQOPaAjrOyH4zrxISU7osqKKLQwsx1PmqAGzKA4Ds7-q6e0LK-O4NGI51TYccNoC5e1kEftSl-yimcnX1qDYMpu_aMj3T80ILiXaxE3GB0bIoecJQgFXv11pC2xnHZytOQ2QkdUR6CWg0lFP8N6NIY9SctPMTqfksoquWvqMwSA6wqwor0D5SpkM_qMzH8l9vM1TpmIMrFKM-pc08AERwoFYna1dYMN-vNujJwBfcN4DIUMg0I3kA8zesIZ575j0X8z0

{
  "alg": "RS256"
}
{
  "qt-demo-item-1_errorKey": "notAuthorized",
  "iss": "e0fb530b-27a3-4bf5-9371-5e67d9691590",
  "qt-demo-item-1_errorCode": "notAuthorized",
  "qt-demo-item-1_errorTechnical": "Authorization failed for \"qt-demo-item-1\"",
  "qt-demo-item-1_errorMessage": "Access to \"qt-demo-item-1\" denied",
  "exp": 1664888094,
  "iat": 1664801694
}
*/
