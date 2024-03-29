#include <QCoreApplication>

#include "QDBusP2PExample/dbus.hpp"
#include "QDBusP2PExample/init.hpp"
#include "QDBusP2PExample/io_github_blackdesk_QDBusP2PExample_Server.h"

using namespace QDBusP2PExample;

auto main(int argc, char **argv) -> int
{
        QCoreApplication app(argc, argv);
        initQCoreApplication();

        auto ret = QMetaObject::invokeMethod(QCoreApplication::instance(), []() {
                auto conn = QDBusConnection::connectToPeer(
                        "unix:path=" + getPeerToPeerSocketAddress(),
                        "p2p DBus connection to server");
                if (!conn.isConnected()) {
                        qCritical() << conn.name()
                                    << "is not connected:" << conn.lastError();
                        QCoreApplication::exit(-1);
                        return;
                }

                auto server =
                        new io::github::blackdesk::QDBusP2PExample::Server(
                                "",
                                "/io/github/blackdesk/QDBusP2PExample/Server",
                                conn, QCoreApplication::instance());

                if (!server->isValid()) {
                        qCritical() << server->lastError();
                        QCoreApplication::exit(-1);
                        return;
                }

                QObject::connect(server,
                                 &io::github::blackdesk::QDBusP2PExample::
                                         Server::TestSignal,
                                 []() {
                                         qInfo() << "TestSignal arrived";
                                         QCoreApplication::exit();
                                 });

                QJsonObject object;
                object.insert("a", "b");
                auto reply = server->TestMethod(object.toVariantMap());
                reply.waitForFinished();
                if (reply.isError()) {
                        qCritical() << "TestMethod error.";
                        QCoreApplication::exit(-1);
                        return;
                }
                qInfo() << "reply:" << reply.value();
        });

        Q_ASSERT(ret);

        return QCoreApplication::exec();
}
