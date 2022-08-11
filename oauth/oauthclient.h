#ifndef OAUTHCLIENT_H
#define OAUTHCLIENT_H

#include "./oauthrequest.h"

#include <map>
#include <memory>

namespace tenduke { namespace oauth {


/** OAuth-client to initiate OAuth authorization flow.
 *  The client creates an OAuthRequest, which handles the flow.
 *
 *  IMPORTANT NOTE: There is no browser integration provided, the implementors must bring their own browser.
 *  This is intentional to prevent large browser-dependency if the implementor wants to use another browser.
 *
 *  The OAuth-service works both with embedded browser and OS-browser. With embedded browser:
 *  1. open embedded browser
 *  2. call OAuthPKCEFlow::authorize() to create OAuthRequest
 *  3. call OAuthRequest::buildAuthorizationRequestUrl() to create URL
 *  4. navigate the embedded browser to the URL create in step #3
 *  5. listen navigations of the embedded browser. The OAuth-server performs series of page-navigations and
 *     on success (or failure) navigates (with HTTP-redirect) to configured callback-URL. You must detect this
 *     navigation, which depends entirely on the embedded browser: Some browsers might provide a hook to get notified
 *     by navigation. In QT WebEngine, you can register custom URL-scheme and get notified when this custom scheme is
 *     navigated to.
 *  6. Once you detect that the callback-URL is navigated to (e.g. via custom scheme), call
 *     OAuthRequest::handleCallback().
 *  7. The OAuthRequest::handleCallback() completes the flow and returns OAuthState.
 *
 *  Steps for OS-browser are similar, but OS-specific. In many cases application can register custom URL-scheme
 *  to the OS and use OS-calls to open the browser and navigate.
 *
 */
class OAuthClient
{
public:
    virtual ~OAuthClient() {}

    /** Creates OAuthRequest to start the OAuth-authorization flow.
     *
     *  @param scopes scopes to authorize
     *  @param additionalParameters additional parameters for the request
     *  @return the OAuth-request
     */
    virtual std::unique_ptr<const tenduke::oauth::OAuthRequest> authorize(
            const std::string &scopes,
            const std::map<std::string, std::string> &additionalParameters
    ) = 0;
};


}}

#endif // OAUTHCLIENT_H
