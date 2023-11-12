#include "QDBusP2PExample/Server.hpp"

#include <QDebug>

namespace QDBusP2PExample
{

void Server::TestMethod()
{
        qInfo() << "method called";
        this->TestSignal();
        return;
}

}
