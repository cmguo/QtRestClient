#ifndef QDEBUGSIGNAL_H
#define QDEBUGSIGNAL_H

#include "QtRestClient_global.h"

#include <QMetaObject>
#include <QMetaMethod>
#include <QLibrary>
#include <QDebug>

// QSignalSpyCallbackSet is defined in qt5/qtbase/src/corelib/kernel/qobject_p.h

struct QSignalSpyCallbackSet
{
    typedef void (*BeginCallback)(QObject *caller, int signal_or_method_index, void **argv);
    typedef void (*EndCallback)(QObject *caller, int signal_or_method_index);
    BeginCallback signal_begin_callback,
                    slot_begin_callback;
    EndCallback signal_end_callback,
                slot_end_callback;
};
typedef void (*register_spy_callbacks)(const QSignalSpyCallbackSet &callback_set);

static int signalToMethod(const QMetaObject *metaObject, int index)
{
    int mc = metaObject->methodOffset();
    while (index < mc) {
        metaObject = metaObject->superClass();
        mc = metaObject->methodOffset();
    }
    int sc = 0;
    for (int i = 0; i < mc; ++i)
        if (metaObject->method(i).methodType() == QMetaMethod::Signal)
            ++sc;
    return index + mc - sc;
}

static void showObject(QObject *caller, int signal_index, const QString &msg, bool signal)
{
    if (!caller->inherits("QAxObject"))
        return;
    const QMetaObject *metaObject = caller->metaObject();
    if (signal)
        signal_index = signalToMethod(metaObject, signal_index);
    QMetaMethod member = metaObject->method(signal_index);
    qDebug() << metaObject->className() << static_cast<void*>(caller) << msg << qPrintable(member.name());
}

static void onSignalBegin(QObject *caller, int signal_index, void **argv)
{
    (void)argv;
    showObject(caller, signal_index, "onSignalBegin", true);
}

static void onSlotBegin(QObject *caller, int signal_index, void **argv)
{
    (void)argv;
    showObject(caller, signal_index, "onSlotBegin", false);
}

void qDebugSignals()
{
    static QSignalSpyCallbackSet set = { onSignalBegin, onSlotBegin, nullptr, nullptr };
    QLibrary qtcore("Qt5Cored");
    register_spy_callbacks reg = reinterpret_cast<register_spy_callbacks>(
                qtcore.resolve("?qt_register_signal_spy_callbacks@@YAXAEBUQSignalSpyCallbackSet@@@Z"));

    if (reg) {
        reg(set);
    }
}

#endif // QDEBUGSIGNAL_H
