#include "./customschemehandler.h"

#include <QDebug>
#include <QWebEngineProfile>
#include <QWebEngineUrlScheme>


namespace qtwe = tenduke::qt::webengine;

qtwe::CustomSchemeHandler::CustomSchemeHandler(const QByteArray &name, QObject * parent)
    : QWebEngineUrlSchemeHandler(parent), scheme(name)
{
    scheme.setSyntax(QWebEngineUrlScheme::Syntax::Host);
    scheme.setDefaultPort(QWebEngineUrlScheme::PortUnspecified);
    scheme.setFlags(QWebEngineUrlScheme::SecureScheme);
    QWebEngineUrlScheme::registerScheme(scheme);
}

void qtwe::CustomSchemeHandler::requestStarted(QWebEngineUrlRequestJob *request)
{
    QUrl url = request->requestUrl();

    qDebug() << "CustomSchemeHandler.requestStarted()" << url.toString() << "\n";

    Q_EMIT customUrlNavigated(url);
}

void qtwe::CustomSchemeHandler::setup()
{
    QWebEngineProfile::defaultProfile()->installUrlSchemeHandler(scheme.name(), this);
}
