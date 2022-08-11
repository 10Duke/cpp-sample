#ifndef CUSTOM_SCHEME_HANDLER_H
#define CUSTOM_SCHEME_HANDLER_H

#include <QWebEngineUrlRequestJob>
#include <QWebEngineUrlSchemeHandler>
#include <QWebEngineUrlScheme>

namespace tenduke { namespace qt { namespace webengine {


/** Handles custom schemes with QTWebEngine.
 *
 *  Whenever the browser navigates to custom URL, `customUrlNavigated` signal is emitted.
 *
 *  IMPORTANT: This class is setup in two phases:
 *    1. Construct instance BEFORE QApplication is instantiated
 *    2. Call setup() before you initialize QtWebEngine.
 */
class CustomSchemeHandler : public QWebEngineUrlSchemeHandler
{
    Q_OBJECT

public:
    /**
     * Constructs new instance.
     *
     * @param name name of the schema, etc. "".
     */
    CustomSchemeHandler(const QByteArray &name, QObject *parent = nullptr);

    /**
     * Registers this instance to handle the custom scheme.
     *
     */
    void setup();

    /**
     * Implements the QWebEngineUrlSchemeHandler: emits customUrlNavigated() with the request URL.
     *
     */
    void requestStarted(QWebEngineUrlRequestJob *request);

Q_SIGNALS:

    /**
     * Emitted when URL with the configured custom scheme is navigated to.
     *
     */
    void customUrlNavigated(QUrl url);

private:
    QWebEngineUrlScheme scheme;
};


}}}

#endif // CUSTOM_SCHEME_HANDLER
