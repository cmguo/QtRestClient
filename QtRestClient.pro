QT -= gui
QT += network

TEMPLATE = lib
DEFINES += QTRESTCLIENT_LIBRARY

CONFIG += c++11

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
    qrestinterceptor.cpp \
    qrestloginterceptor.cpp \
    qrestrequest.cpp \
    qrestretryinterceptor.cpp \
    qrestservice.cpp \
    qtestdata.cpp

HEADERS += \
    QtRestClient_global.h \
    qdebugsignal.h \
    qjsonwrapperconvertor.h \
    qrestarg.h \
    qrestclient.h \
    qrestexception.h \
    qrestinterceptor.h \
    qrestjson.h \
    qrestjsonresultconverter.h \
    qrestloginterceptor.h \
    qrestrequest.h \
    qrestretryinterceptor.h \
    qrestservice.h \
    qtestdata.h

includes.files = $$PWD/*.h $$PWD/*.hpp
win32 {
    includes.path = $$[QT_INSTALL_HEADERS]/QtRestClient
    target.path = $$[QT_INSTALL_LIBS]
}
INSTALLS += includes

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/../QtPromise/src

INCLUDEPATH += $$PWD/../qtpromise/src/qtpromise $$PWD/../qtpromise/include
DEPENDPATH += $$PWD/../qtpromise/src/qtpromise

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QtJsonSerializer/lib/ -lQt5JsonSerializerd
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QtJsonSerializer/lib/ -lQt5JsonSerializerd
else:unix: LIBS += -L$$OUT_PWD/../QtJsonSerializer/lib/ -lQt5JsonSerializerd

INCLUDEPATH += $$OUT_PWD/../QtJsonSerializer/include/
