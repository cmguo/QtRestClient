QT -= gui
QT += network

TEMPLATE = lib
DEFINES += QTRESTCLIENT_LIBRARY

CONFIG += c++17

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
    qresttimeoutinterceptor.cpp \
    qtestdata.cpp

HEADERS += \
    QtRestClient_global.h \
    qdebugsignal.h \
    qjsonwrapperconvertor.h \
    qrestarg.h \
    qrestbean.h \
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
    qresttimeoutinterceptor.h \
    qtestdata.h

includes.files = $$PWD/*.h $$PWD/*.hpp
includes.path = $$[QT_INSTALL_HEADERS]/QtRestClient
target.path = $$[QT_INSTALL_LIBS]

INSTALLS += includes

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

include($$(applyConanPlugin))

QMAKE_CXXFLAGS += /utf-8

CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,d)
}

CONFIG(release, debug|release) {
    QMAKE_CXXFLAGS+=/Zi
    QMAKE_LFLAGS+= /INCREMENTAL:NO /Debug
}