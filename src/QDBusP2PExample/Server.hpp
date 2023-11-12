#pragma once

#include <QDBusContext>
#include <qobjectdefs.h>

namespace QDBusP2PExample
{

class Server : public QObject, public QDBusContext {
        Q_OBJECT
        using QObject::QObject;

    public:
        virtual void TestMethod();

    Q_SIGNALS:
        void TestSignal();
};
}
