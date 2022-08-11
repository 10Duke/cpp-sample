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
    std::shared_ptr<json::JSONParser> jsonParser,
    std::shared_ptr<http::HTTPClient> httpClient,
    std::shared_ptr<const licensing::LicensingConfiguration> config,
    std::unique_ptr<const licensing::LicenseCheckoutParameters> params
) : parseResponse(jsonParser), http(httpClient), config(config), params(std::move(params)),
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
    urlBuilder.baseURL(config->getEndpoint() + ".json");

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
    std::unique_ptr<json::JSONElement> response = parseResponse->from(payload.c_str());

    // Expect JSON object response:
    if (! response->isObject()) {
        throw licensing::LicensingRequestFailure("Invalid response, expected JSON object");
    }

    std::map<std::string, std::string> itemNameLookup;

    // Build lookup structure:
    // item prefixes: qt-demo-item-1 --> qt-demo-item-1_
    for (auto const &item : params->getItems()) {
        itemNameLookup.emplace(item, item + '_');
    }

    std::map<std::string, licensing::LicenseCheckoutItem> items;
    std::map<std::string, licensing::LicenseCheckoutError> failedItems;

    // Loop through the JSON properties:
    json::JSONObject * responseObject = dynamic_cast<json::JSONObject *>(response.get());
    for (auto const &property : responseObject->getProperties()) {
        const std::string &key = property.first;
        const std::shared_ptr<json::JSONElement> & value = property.second;

        // Check for licensed item with exact match.
        // Example: "qt-demo-item-1": true
        if (itemNameLookup.count(key) > 0) {
            // Successfully checked out
            if (json::isBooleanTrue(value)) {
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
                        failedItem.setErrorCode(value->asString());
                    } else if (property == "errorKey") {
                        failedItem.setErrorKey(value->asString());
                    } else if (property == "errorMessage") {
                        failedItem.setErrorMessage(value->asString());
                    } else if (property == "errorTechnical") {
                        failedItem.setErrorTechnical(value->asString());
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
*/
