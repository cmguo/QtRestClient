QT -= gui
QT += network

CONFIG += c++14 console
CONFIG -= app_bundle

include($$(applyCommonConfig))
include($$(applyConanPlugin))

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
    main.cpp \
    qtestdata.cpp

HEADERS += \
    qtestdata.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ -lQtRestClient
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ -lQtRestClientd
else:unix: LIBS += -L$$OUT_PWD/../ -lQtRestClient

INCLUDEPATH += $$PWD/../QtRestClient
DEPENDPATH += $$PWD/../QtRestClient
