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
    qrestproxyinterceptor.h \
    qrestrequest.h \
    qrestretryinterceptor.h \
    qrestservice.h \
    qresttimeoutinterceptor.h \
    qtestdata.h

CONFIG(debug, debug|release) {
    win32: TARGET = $$join(TARGET,,,d)
}

msvc:CONFIG(release, debug|release) {
    QMAKE_CXXFLAGS+=/Zi
    QMAKE_LFLAGS+= /INCREMENTAL:NO /Debug
    target2.files = $$OUT_PWD/release/QtRestClient.pdb
    target2.path = $$[QT_INSTALL_LIBS]
    INSTALLS += target2
}

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QtJsonSerializer/lib/ -lQt5JsonSerializer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QtJsonSerializer/lib/ -lQt5JsonSerializerd
else:mac: LIBS += -F$$OUT_PWD/../QtJsonSerializer/lib -framework QtJsonSerializer
else:unix: LIBS += -L$$OUT_PWD/../QtJsonSerializer/lib/ -lQt5JsonSerializer

INCLUDEPATH += $$OUT_PWD/../QtJsonSerializer/include/

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../log4qt/release/ -llog4qt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../log4qt/debug/ -llog4qtd
else:unix: LIBS += -L$$OUT_PWD/../log4qt/ -llog4qt

INCLUDEPATH += $$PWD/../log4qt
INCLUDEPATH += $$PWD/../log4qt/src
DEPENDPATH += $$PWD/../log4qt
