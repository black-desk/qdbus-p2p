#pragma once

#include <QDBusContext>
#include <qobjectdefs.h>

namespace QDBusP2PExample
{

class Server : public QObject, public QDBusContext {
        Q_OBJECT
        using QObject::QObject;

    public:
        virtual QVariantMap TestMethod(const QVariantMap &parameters);

    Q_SIGNALS:
        void TestSignal();
};
}
