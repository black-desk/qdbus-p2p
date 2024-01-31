#include "QDBusP2PExample/Server.hpp"

#include <QDebug>

namespace QDBusP2PExample
{

QVariantMap Server::TestMethod(const QVariantMap &parameters)
{
        qInfo() << "method called" << parameters;
        this->TestSignal();
        return {};
}

}
