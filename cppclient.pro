#-------------------------------------------------
#
# Project created by QtCreator 2019-11-12T09:56:03
#
#-------------------------------------------------

QT       += core gui network networkauth webengine webenginewidgets widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cppclientdemo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    crypto/asymmetricmessagedigest.cpp \
    crypto/asymmetricmessagedigestfactory.cpp \
    crypto/createlibcryptoexception.cpp \
    crypto/libcryptomessagedigest.cpp \
    crypto/publickey.cpp \
    crypto/rsapublickeyfrompemstring.cpp \
    crypto/sha256utl.cpp \
    demo/basewindow.cpp \
    demo/demoparametersdialog.cpp \
    demo/licensecheckoutwindow.cpp \
    demo/logindialog.cpp \
    demo/mainwindow.cpp \
    demo/userinfowindow.cpp \
    http/httprequestbuilder.cpp \
    http/httpresponse.cpp \
    http/httpstatuscodetoexception.cpp \
    http/libcurlhttpcall.cpp \
    http/libcurlhttpclient.cpp \
    http/testhttpclient.cpp \
    http/testlibcurlhttpclient.cpp \
    http/urlbuilder.cpp \
    json/cjsonarray.cpp \
    json/cjsonelement.cpp \
    json/cjsonobject.cpp \
    json/cjsonparser.cpp \
    json/jsonutils.cpp \
    jwt/defaultjwtparser.cpp \
    jwt/fixedkeyjwtsignaturevalidator.cpp \
    jwt/jwt.cpp \
    jwt/testjwt.cpp \
    licensing/defaultlicensecheckoutrequest.cpp \
    licensing/defaultlicensingclient.cpp \
    licensing/licensecheckoutparametersbuilder.cpp \
    licensing/licensecheckoutresponse.cpp \
    licensing/licensinghttpstatuscodetoexception.cpp \
    licensing/testlicensing.cpp \
    main.cpp \
    net/libcurlurl.cpp \
    net/libcurlurldecoder.cpp \
    net/libcurlurlencoder.cpp \
    net/libcurlurlparser.cpp \
    net/testurl.cpp \
    oauth/oauthpkceflow.cpp \
    oauth/oauthpkcerequest.cpp \
    oauth/oauthstateimpl.cpp \
    oauth/oauthtokenrequest.cpp \
    oidc/createdefaultoidcclient.cpp \
    oidc/oidcclient.cpp \
    oidc/oidcrequest.cpp \
    qt/http/createqthttpclient.cpp \
    qt/http/qthttpcall.cpp \
    qt/http/qthttpclient.cpp \
    qt/http/testqthttpclient.cpp \
    qt/json/qtjsonarray.cpp \
    qt/json/qtjsonelement.cpp \
    qt/json/qtjsonobject.cpp \
    qt/json/qtjsonparser.cpp \
    qt/json/qtjsonprimitive.cpp \
    qt/licensing/createqtlicensingclient.cpp \
    qt/licensing/qtlicensecheckoutrequest.cpp \
    qt/licensing/qtlicensingclient.cpp \
    qt/net/qturl.cpp \
    qt/net/qturlencoder.cpp \
    qt/net/qturlparser.cpp \
    qt/oidc/createqtoidcclient.cpp \
    qt/oidc/qtoidcclient.cpp \
    qt/utl/exceptiontostringlist.cpp \
    qt/utl/qtbase64decoder.cpp \
    qt/utl/qtbase64encoder.cpp \
    qt/utl/random/qtrandombytes.cpp \
    qt/webengine/customschemehandler.cpp \
    time/defaultclock.cpp \
    tst/testdefaultlicensing.cpp \
    utl/base64test.cpp \
    utl/defaultbase64decoder.cpp \
    utl/defaultbase64encoder.cpp \
    utl/fixedsizebinarydata.cpp \
    utl/random/insecurerandombytes.cpp \
    utl/random/randomurlsafestring.cpp \
    utl/simplebuffer.cpp
 
HEADERS += \
    crypto/asymmetricmessagedigest.h \
    crypto/asymmetricmessagedigestfactory.h \
    crypto/createlibcryptoexception.h \
    crypto/cryptoexception.h \
    crypto/libcryptomessagedigest.h \
    crypto/messagedigest.h \
    crypto/messagedigestverifier.h \
    crypto/messagedigestverifierfactory.h \
    crypto/publickey.h \
    crypto/rsapublickeyfrompemstring.h \
    crypto/sha256utl.h \
    demo/basewindow.h \
    demo/demoparametersdialog.h \
    demo/licensecheckoutwindow.h \
    demo/logindialog.h \
    demo/mainwindow.h \
    demo/userinfowindow.h \
    http/badrequest.h \
    http/clienterror.h \
    http/forbidden.h \
    http/httpcall.h \
    http/httpclient.h \
    http/httpexception.h \
    http/httprequest.h \
    http/httprequestbuilder.h \
    http/httprequestexception.h \
    http/httpresponse.h \
    http/httpresponseexception.h \
    http/httpstatuscodetoexception.h \
    http/libcurlhttpcall.h \
    http/libcurlhttpclient.h \
    http/notfound.h \
    http/servererror.h \
    http/testhttpclient.h \
    http/testlibcurlhttpclient.h \
    http/unauthorized.h \
    http/urlbuilder.h \
    json/cjsonarray.h \
    json/cjsonboolean.h \
    json/cjsonelement.h \
    json/cjsonnumber.h \
    json/cjsonobject.h \
    json/cjsonparser.h \
    json/cjsonstring.h \
    json/jsonarray.h \
    json/jsonboolean.h \
    json/jsonelement.h \
    json/jsonnumber.h \
    json/jsonobject.h \
    json/jsonparser.h \
    json/jsonparsingexception.h \
    json/jsonstring.h \
    json/jsonutils.h \
    jwt/defaultjwtparser.h \
    jwt/fixedkeyjwtsignaturevalidator.h \
    jwt/jwt.h \
    jwt/jwtparser.h \
    jwt/jwtparsingexception.h \
    jwt/jwtsignaturevalidator.h \
    jwt/jwtvalidationexception.h \
    jwt/testjwt.h \
    licensing/defaultlicensecheckoutrequest.h \
    licensing/defaultlicensingclient.h \
    licensing/licensecheckouterror.h \
    licensing/licensecheckoutitem.h \
    licensing/licensecheckoutparameters.h \
    licensing/licensecheckoutparametersbuilder.h \
    licensing/licensecheckoutrequest.h \
    licensing/licensecheckoutresponse.h \
    licensing/licensingclient.h \
    licensing/licensingconfiguration.h \
    licensing/licensingexception.h \
    licensing/licensinghttpstatuscodetoexception.h \
    licensing/licensingrequestfailure.h \
    licensing/testlicensing.h \
    net/libcurlurl.h \
    net/libcurlurldecoder.h \
    net/libcurlurlencoder.h \
    net/libcurlurlparser.h \
    net/malformedurlexception.h \
    net/networkingexception.h \
    net/requesttimedout.h \
    net/testurl.h \
    net/unknownhostexception.h \
    net/url.h \
    net/urldecoder.h \
    net/urlencoder.h \
    net/urlexception.h \
    net/urlparser.h \
    oauth/oauthclient.h \
    oauth/oauthconfiguration.h \
    oauth/oauthexception.h \
    oauth/oauthpkceflow.h \
    oauth/oauthpkcerequest.h \
    oauth/oauthrequest.h \
    oauth/oauthservererror.h \
    oauth/oauthstate.h \
    oauth/oauthstateimpl.h \
    oauth/oauthtokenrequest.h \
    oauth/oauthtokenresponse.h \
    oidc/createdefaultoidcclient.h \
    oidc/oidcclient.h \
    oidc/oidcconfiguration.h \
    oidc/oidcexception.h \
    oidc/oidcrequest.h \
    oidc/oidcstate.h \
    qt/http/createqthttpclient.h \
    qt/http/qthttpcall.h \
    qt/http/qthttpclient.h \
    qt/http/testqthttpclient.h \
    qt/json/qtjsonarray.h \
    qt/json/qtjsonboolean.h \
    qt/json/qtjsonelement.h \
    qt/json/qtjsonnumber.h \
    qt/json/qtjsonobject.h \
    qt/json/qtjsonparser.h \
    qt/json/qtjsonprimitive.h \
    qt/json/qtjsonstring.h \
    qt/licensing/createqtlicensingclient.h \
    qt/licensing/qtlicensecheckoutrequest.h \
    qt/licensing/qtlicensingclient.h \
    qt/licensing/qtlicensingconfiguration.h \
    qt/net/qturl.h \
    qt/net/qturlencoder.h \
    qt/net/qturlparser.h \
    qt/oauth/qtoauthconfig.h \
    qt/oauth/qtoauthstate.h \
    qt/oidc/createqtoidcclient.h \
    qt/oidc/qtoidcclient.h \
    qt/oidc/qtoidcconfig.h \
    qt/oidc/qtoidcstate.h \
    qt/utl/exceptiontostringlist.h \
    qt/utl/qtbase64decoder.h \
    qt/utl/qtbase64encoder.h \
    qt/utl/qtdeletelaterfunction.h \
    qt/utl/random/qtrandombytes.h \
    qt/webengine/customschemehandler.h \
    time/clock.h \
    time/defaultclock.h \
    tst/testdefaultlicensing.h \
    utl/base64decoder.h \
    utl/base64encoder.h \
    utl/base64test.h \
    utl/binarydata.h \
    utl/defaultbase64decoder.h \
    utl/defaultbase64encoder.h \
    utl/fixedsizebinarydata.h \
    utl/random/insecurerandombytes.h \
    utl/random/randombytes.h \
    utl/random/randomurlsafestring.h \
    utl/simplebuffer.h
 
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    demo/demoparametersdialog.ui \
    demo/licensecheckoutwindow.ui \
    demo/mainwindow.ui \
    demo/userinfowindow.ui

DISTFILES +=

RESOURCES +=

#unix|win32: LIBS += -lcrypto -lcurl -lcjson
unix|win32: LIBS += -lcrypto -lcurl

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libcjson
