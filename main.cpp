#include <QApplication>
#include <qtwebengineglobal.h>

#include "demo/mainwindow.h"
#include "qt/webengine/customschemehandler.h"

#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

// IMPORTANT: This must match the scheme of the redirect-URI configured in the OAuth2-backend
static const QByteArray CUSTOM_SCHEME = "tenduke";

void handler(int sig) {
  void *array[50];
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 50);

  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, STDERR_FILENO);
  exit(1);
}

int main(int argc, char *argv[])
{
    signal(SIGSEGV, handler);   // install our handler

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    std::shared_ptr<tenduke::qt::webengine::CustomSchemeHandler> schemeHandler (new tenduke::qt::webengine::CustomSchemeHandler(CUSTOM_SCHEME));

    QCoreApplication::setOrganizationName("10Duke SSO demo");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    schemeHandler->setup();

    QtWebEngine::initialize();
#else
    // Initialize QtWebEngine first. Initialization order is essential.
    QtWebEngine::initialize();
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // Setup the application:
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("10Duke SSO demo");

    // Initialize and register the custom scheme handler.
    std::shared_ptr<tenduke::qt::webengine::CustomSchemeHandler> schemeHandler (new tenduke::qt::webengine::CustomSchemeHandler(CUSTOM_SCHEME));
    schemeHandler->setup();
#endif


    tenduke::qt::demo::MainWindow mainWindow(
                schemeHandler,
                std::shared_ptr<QNetworkAccessManager>(new QNetworkAccessManager()),
                nullptr
    );
    mainWindow.show();

    return app.exec();
}

/*
#include "tst/testdefaultlicensing.h"

int main (int argc, char *argv[])
{
    tenduke::tst::licensing::testDefaultLicensingWithAutoDiscovery();

    return 0;
}
*/
/*
#include "qt/http/testqthttpclient.h"
#include "http/testlibcurlhttpclient.h"

int main (int argc, char *argv[])
{
    QApplication app(argc, argv);

    tenduke::http::curl::testLibCurlHTTPClient();
    tenduke::qt::http::testQtHttpClient();

    return 0;
}
*/
