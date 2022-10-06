# C++ / QT-demo for 10Duke licensing

This is a C++-based demo, which handles user login and license checkout.
The executable built from this is a simple QT-demo.
The demo contains generic C++ implementation (minus browser) and QT-based implementation (with QT embedded WebBrowser).
This example/demo is not a full-fledged client-library but could be used as a base for one.

Some of the functionality use service-interfaces, which have either QT-based implementations or default-implementations
with some additional libraries. This allows the implementor to choose libraries he wants and implement the interfaces
with their preferred libraries. Some examples of implementation-specific services are:
- `tenduke::http::HTTPClient`, simple HTTP-client. This demo contains QT and libcurl-based implementations.
- `tenduke::json::JSONParser`, simple JSON-parser. This demo contains QT and cjson-based implementations.
- `tenduke::crypto`-classes, these have libcrypto-based implementations

Note that the above services are not complete implementations (e.g. HTTPClient is not a do-everything general-purpose
HTTP-client), but typically implement just enough services that the required tasks get done.

Most of the functionality is in generic C++-code and the QT-code is mostly for implementing certain service-interfaces
with QT libraries and QT-specific binding. The generic C++-part in most cases uses exceptions for error conditions,
while the QT-part uses QT-signals. The error data signalled in QT are actually exceptions so that the application
developer can react appropriately depending on the error.


## Authenticating the user

To use 10Duke licensing, the user needs to be authenticated and for that 10Duke uses OpenID Connect (OIDC), which
is OAuth-based authentication protocol. And being OAuth-based authentication protocol, the application needs to
perform the authentication using browser.

The most important result of the authentication is an access token, which is used to identify the user for the
licensing API. Depending on the configuration, the access token usually expires after a certain period of time.
The access token can be refreshed for some period of time without needing the user to re-authenticate. Note that
the user will eventually need to re-authenticate when the refresh fails with an error: The OIDC session has a
limited lifetime.


### Authenticating the user with the generic C++

The generic C++-part of this demo does not contain browser integration, to allow the application developer to choose
the browser for embedding or to use the OS-browser. The browser integration depends on which kind of and what browser
you want to use: Embedded or OS and if embedded, then the type of the browser.

The OAuth-server will notify of success (or error) by navigating with HTTP redirect to a pre-agreed URL, called
callback-URL or redirect-URL. This URL will be configured with the OAuth/OIDC provider. You need to be able to
detect when the callback-URL is navigated to. Most browsers and operating systems allow you to hook some sort of
navigation listener. Some examples:
 - Some browsers might allow you to listen any navigation
 - Some browsers (and operating systems) allow hook a "custom-URL-scheme"-listener, which is called whenever
   the navigation is done to a URL which has special URL-scheme, e.g. in "this-is-callback://host/path" the
   custom scheme would be "this-is-callback" and whenever navigation is done to such URL, you would get notified
 - Some browsers allow you to listen to failed navigation, in which case you could define the callback URL
   so that the navigation fails.

In order to authenticate the user with generic C++-client, you need to do something like following (full example in
tst/testdefaultlicensing.cpp):

```c++
#include "oidc/oidcclient.h"
#include "oidc/createdefaultoidcclient.h"
...

...
  //  Default services needed:
  std::shared_ptr<xdhttp::HTTPClient> httpClient (new xdcurlhttp::LibCurlHTTPClient());
  std::shared_ptr<xdjson::JSONParser> jsonParser (new xdcjson::cJSONParser());
  std::shared_ptr<xdtime::Clock> clock (new xdtime::DefaultClock());
  std::shared_ptr<xdrandom::RandomBytes> randomGenerator (new xdrandom::InsecureRandomBytes());
  std::shared_ptr<xdutl::Base64Decoder> base64Decoder (new xdutl::DefaultBase64Decoder());

  // Auto-discover OIDC backend configuration
  xdoidc::AutoDiscovery oidcConfiguration(base64Decoder, httpClient, jsonParser);
  xdoidc::AutoDiscoveryResult discoveredCfg = oidcConfiguration.discover(OIDC_AUTO_DISCOVERY_URL);
  std::shared_ptr<const xdoidc::OIDCConfiguration> discoveredOIDCCfg = oidcConfiguration.getOIDCConfiguration(discoveredCfg);
  
  // Construct JWT parser
  std::shared_ptr<const xdjwt::JWTParser> jwtParser = xdjwt::createJWTParser(
      discoveredOIDCCfg->algorithm,
      discoveredOIDCCfg->verificationKey,
      base64Decoder,
      jsonParser
  );
  
  // Make the OAuth-config from the auto-discovered backend configuration plus the client configuration:
  std::shared_ptr<const xdoauth::OAuthConfiguration> oauthConfig (new xdoauth::OAuthConfiguration(
      *discoveredCfg.getOAuthConfiguration().get(),
      xdoauth::OAuthClientConfiguration(
          OAUTH_CLIENT_ID,          // fill-in the OAuth client id set configured with the OAuth provider
          OAUTH_CLIENT_SECRET,      // fill-in the OAuth client secret configured with the OAuth provider
          OAUTH_CALLBACK_URI,       // fill-in the OAuth redirect-URI configured with the OAuth provider
          true                      // use PKCE
      )
  ));

  // Create OIDC-client:
  std::unique_ptr<tenduke::oauth::oidc::OIDCClient> oidcClient = tenduke::oauth::oidc::createDefaultOIDCClient(
      oauthConfig,
      discoveredOIDCCfg
      httpClient,
      jsonParser,
      jwtParser,
      randomGenerator,
      clock
  );

  // Start the OIDC-flow:
  // IMPORTANT: Keep the oidcRequest during the entire authentication, it contains the state of the OIDC-flow
  std::unique_ptr<const tenduke::oauth::oidc::OIDCRequest> oidcRequest = oidcClient->authenticate("profile email");
  
  // IMPORTANT: Open the browser here. You need to listen the navigation as described above.
  
  // IMPORTANT: Once browser is open navigate the browser to following URL:
  std::string url = oidcRequest->buildAuthenticationRequestUrl();

  // IMPORTANT: Once the authentication succeeds or fails, the callback URL will be navigated to.
  // Pass the complete callback URL:
  
  std::string callbackUrl = ...; // Callback URL from the browser
  
  // This now completes the authentication
  // After successful login, the tenduke::oauth::oidc::OIDCState contains the information which proves
  // that the user has successfully authenticated himself.
  // Error conditions would throw exceptions.
  std::unique_ptr<const tenduke::oauth::oidc::OIDCState> oidcState = oidcRequest->handleCallback(callbackUrl);
```


### Authenticating the user with QT

IMPORTANT: If you change the URL-scheme used in OAuth2 callback URI (when you configure the callback URI in
the OAuth2-backend), remember to change the scheme in the source code to match the new scheme. See the
constant `CUSTOM_SCHEME` in `main.cpp`.

The QT-part uses QT WebEngine as embedded browser.

The demo registers a custom URL-scheme "tenduke" to the WebEngine (see `main.cpp`). All the action happens via
QT signals and slots.

NOTE: The QtOIDCClient contains the login state, so keep the instance alive for the session duration. The client
can be used to refresh the OAuth access token.

Example (for complete example see the demo-application in the demo-directory, in particular methods `main()` and
`MainWindow::login()` and class `LoginDialog`):

First, initialize the QWebEngine and custom-scheme handler before you start the QApplication (note that the
initialization order seems to be dependent on the QT-version, see main.cpp):

```c++
  // Create custom scheme handler:
  std::shared_ptr<tenduke::qt::webengine::CustomSchemeHandler> schemeHandler (new tenduke::qt::webengine::CustomSchemeHandler("tenduke"));
  schemeHandler->setup();
  QtWebEngine::initialize();
```

Then, in your login - window, do something like following:
```c++
  // Note: QNetworkAccessManager instance should probably be application-wide singleton
  std::shared_ptr<QNetworkAccessManager> networkAccessManager (new QNetworkAccessManager());
  std::shared_ptr<tenduke::http::HttpClient> httpClient = tenduke::qt::http::createQtHTTPClient(networkAccessManager);
  
  // Create the OIDC-client
  std::shared_ptr<tenduke::oauth::oidc::QtOIDCClient> oidcClient = tenduke::qt::oidc::createOidcClient(
      oidcParameters, // setup the parameters, not shown here
      httpClient
  );
  
  // Setup the QWebEngineView somehow
  QWebEngineView browser ... ;
  
  // When the OAuth-flow wants to open browser, instruct the embedded browser to load the authorization URL:
  QObject::connect(
      oidcClient.get(), &tenduke::qt::oidc::QtOIDCClient::openBrowser,
      &browser, &QWebEngineView::setUrl
  );
  
  // When the callback URL is navigated to, the CustomSchemeHandler emits customUrlNavigated()-signal. This
  // is connected to the OAuth flow to so that it can continue:
  QObject::connect(
      schemeHandler.get(), &tenduke::qt::webengine::CustomSchemeHandler::customUrlNavigated,
      oidc.get(), &tenduke::qt::oidc::QtOIDCClient::callbackUrlNavigated
  );

  // Do something when OIDC-authentication succeeds:
  QObject::connect(
      oidc.get(), &tenduke::qt:oidc::QtOIDCClient::authenticated,
      this, &MyClass::loginSuccessful
  );

  // Do something when OIDC-authentication fails (for any reason):
  QObject::connect(
      oidc.get(), &tenduke::qt:oidc::QtOIDCClient::authenticationFailure,
      this, &MyClass::loginFailed
  );

  // Starts the authentication flow
  // - navigates the browser to the authentication URL
  // - once authentication complete in browser, returns to handle callback URL
  // - executes next steps to complete the authentication
  // - signals oidcLoginSucceeded() once everything ready
  oidc->authenticate(QString("profile email")); // Parameters "profile email" are OAuth scopes
   
  // Result of the authentication is tenduke::qt::oidc::QtOIDCState, which describes the login state
  // but you should keep the QtOIDCClient instance to maintain the state and refresh the access token.
```


## Checking out licenses

After the user has been successfully authenticated, licenses can be checked out. For this, an OAuth access token 
is needed. Successful login result contains the access token. If OAuth is not familiar to you, you can think of
the access token as a token that identifies the user.

Note that even if none of the items can be successfully checked out, the request is not "failure".
I.e. the client does not throw exception nor does the QT-client signal error. Only technical errors like
network failures or HTTP 4xx/5xx are considered errors. Thus, the application developer must check the
`tenduke::licensing::LicenseCheckoutResponse::getFailedItems()` to determine if any of the request license failed
to check out.


### Generic C++

Complete example in `tst/testdefaultlicensing.cpp`.

```c++
    // Setup the licensing parameters
    std::shared_ptr<tenduke::licensing::LicensingConfiguration> licensingConfiguration (new tenduke::licensing::LicensingConfiguration(
        oidcState->getAccessToken(),          // Access token from the successful login result
        "https://genco.10duke.com/authz/"     // This is the licensing endpoint provided to you by 10Duke
    ));

    // Create the licensing client
    // You can reuse the HTTPClient and JWTParser created earlier
    std::unique_ptr<xdlicensing::LicensingClient> licenses (new xdlicensing::DefaultLicensingClient(
        licensingConfiguration,
        httpClient,
        jwtParser
    ));

    // Checkout the licenses
    // The request is synchronous
    // Errors will be communicated via exceptions
    std::unique_ptr<tenduke::licensing::LicenseCheckoutResponse> checkoutResponse = licenses->checkout(
        tenduke::licensing::LicenseCheckoutParametersBuilder()
            .hardwareId("hw-1")
            .item("qt-demo-item-1")
            .build()
    )->execute();

    // The tenduke::licensing::LicenseCheckoutResponse describes the results, dump them to console
    std::cout << "Successfully checked out items:" << std::endl;
    for (auto const &item : checkoutResponse->getItems()) {
        std::cout << " - " << item.second.getName() << std::endl;
    }

    std::cout << "Failed items:" << std::endl;
    for (auto const &failedItem : checkoutResponse->getFailedItems()) {
        std::cout << " - " << failedItem.first << ":" << std::endl;
        std::cout << "   error code: " << failedItem.second.getErrorCode() << std::endl;
        std::cout << "   error key: " << failedItem.second.getErrorKey() << std::endl;
        std::cout << "   error message: " << failedItem.second.getErrorMessage() << std::endl;
        std::cout << "   tecnical: " << failedItem.second.getErrorTechnical() << std::endl;
    }
```


### QT

```c++
    // Setup the licensing parameters
    tenduke::qt::licensing::QtLicensingConfiguration licensingConfiguration (
        QUrl("https://genco.10duke.com/authz/"),    // This is the licensing endpoint provided to you by 10Duke
        "abc"                                       // Hardware ID: Uniquely identifies this piece of hardware
    );

    std::unique_ptr<const xdjwt::JWTParser> jwtParser = qtjwt::createQtJWTParser(
        "sha256",
        oidcParameters->verificationKey
    );
    licenses = qtlic::createQtLicensingClient(
        *(licensingParameters.get()),
        oidcState->getAccessToken().toStdString(),  // access token from the successful login result
        httpClient,                                 // can be the same as what was used with OIDCClient        
        std::move(jwtParser)
    );

    // Create the checkout request:
    auto request = license->checkout("qt-demo-item-1");
    
    // On success:
    QObject::connect(
        request.get(), &tenduke::qt::licensing::QtLicenseCheckoutRequest::finished,
        this, &MyClass::onSuccessfulCheckout
    );
    
    // On failure:
    QObject::connect(
        request.get(), &tenduke::qt::licensing::QtLicenseCheckoutRequest::failed,
        this, &LicenseCheckoutWindow::checkoutRequestFailed
    );
    
    // Execute the request
    request->execute();
```


## Building

The project has been built with QTCreator and has been tested with Fedora Linux and Ubuntu Linux.

The current project builds and links all the files, so the QT-demo will contain libcurl and cjson, even
though it does not use them. Also test files are currently linked in.

Dependencies:
- libcrypto
- cJson >= 1.7.13 (needed by generic C++-client)
- libcurl (needed by generic C++-client)


### Fedora

```bash
# QT WebEngine
sudo dnf install qt5-qtwebengine-devel

# dependencies:
sudo dnf install \
  cjson-devel \
  openssl-devel \
  libcurl-devel
```


### Ubuntu

```bash
# QT WebEngine
sudo apt-get install \
  libqt5webengine5 \
  qtwebengine5-dev \
  qtwebengine5-dev-tools

# dependencies:
sudo apt-get install \
  libcjson-dev \
  libssl-dev \
  libcurl4-openssl-dev
```

NOTE: If Ubuntu provides too old version of libcjson, here is how to compile manually:

```bash
sudo apt install pkgconf

curl -L -O https://github.com/DaveGamble/cJSON/archive/refs/tags/v1.7.13.tar.gz
tar -xzf v1.7.13.tar.gz
cd cJSON-1.7.13

# Manually compile cJSON libraries are installed under this prefix:
INSTALL_PREFIX=/usr/local
INSTALL_LIBDIR=lib/x86_64-linux-gnu

mkdir build
cd build
cmake .. -DENABLE_CJSON_UTILS=On -DENABLE_CJSON_TEST=Off -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} -DCMAKE_INSTALL_LIBDIR=lib/x86_64-linux-gnu
make
sudo make install
sudo ldconfig
```

