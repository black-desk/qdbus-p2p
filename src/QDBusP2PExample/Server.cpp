#include "QDBusP2PExample/Server.hpp"

#include <QDebug>
#include <QJsonObject>

namespace QDBusP2PExample
{

QVariantMap Server::TestMethod(const QVariantMap &parameters)
{
        auto object = QJsonObject::fromVariantMap(parameters);
        qInfo() << "method called" << object;
        this->TestSignal();
        return object.toVariantMap();
}

}
