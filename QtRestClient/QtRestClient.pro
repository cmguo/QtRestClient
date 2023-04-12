QT -= gui
QT += network

TEMPLATE = lib
DEFINES += QTRESTCLIENT_LIBRARY

CONFIG += c++17

include(../config.pri)

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    qjsonwrapperconvertor.cpp \
    qrestarg.cpp \
    qrestclient.cpp \
    qresthostinterceptor.cpp \
    qrestinterceptor.cpp \
    qrestjson.cpp \
    qrestloginterceptor.cpp \
    qrestmockinterceptor.cpp \
    qrestproxyinterceptor.cpp \
    qrestrequest.cpp \
    qrestretryinterceptor.cpp \
    qrestservice.cpp \
    qresttimeoutinterceptor.cpp

HEADERS += \
    QtRestClient_global.h \
    qdebugsignal.h \
    qjsonwrapperconvertor.h \
    qrestarg.h \
    qrestbean.h \
    qrestboostloginterceptor.h \
    qrestclient.h \
    qrestclient.hpp \
    qrestexception.h \
    qresthostinterceptor.h \
    qrestinterceptor.h \
    qrestjson.h \
    qrestjson.hpp \
    qrestjsonresultconverter.h \
    qrestloginterceptor.h \
    qrestmockinterceptor.h \
    qrestproxyinterceptor.h \
    qrestrequest.h \
    qrestretryinterceptor.h \
    qrestservice.h \
    qrestservice.hpp \
    qresttimeoutinterceptor.h

CONFIG(log4qt) {
DEFINES += HAVE_LOG4QT
SOURCES += \
    qrestlog4qtinterceptor.cpp
HEADERS += \
    qrestlog4qtinterceptor.h
}

CONFIG(boostlog) {
DEFINES += HAVE_BOOSTLOG
SOURCES += \
    qrestboostloginterceptor.cpp
HEADERS += \
    qrestboostloginterceptor.h
}

includes.files = $$PWD/*.h $$PWD/*.hpp
includes.path = $$[QT_INSTALL_HEADERS]/QtRestClient
target.path = $$[QT_INSTALL_LIBS]

INSTALLS += includes

INCLUDEPATH += $$PWD/../../QtPromise/include
DEPENDPATH += $$PWD/../../QtPromise/src/qtpromise

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../QtJsonSerializer/lib/ -lQt5JsonSerializer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../QtJsonSerializer/lib/ -lQt5JsonSerializerd
else:unix: LIBS += -L$$OUT_PWD/../../QtJsonSerializer/lib/ -lQt5JsonSerializer

INCLUDEPATH += $$OUT_PWD/../../QtJsonSerializer/include
DEPENDPATH += $$PWD/../../QtJsonSerializer/src/jsonserializer
