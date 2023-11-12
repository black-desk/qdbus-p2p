#include <iostream>

#include <QCoreApplication>
#include <QDBusServer>
#include <QStringView>

#include "QDBusP2PExample/Server.hpp"
#include "QDBusP2PExample/ServerAdaptor.h"
#include "QDBusP2PExample/dbus.hpp"
#include "QDBusP2PExample/init.hpp"

using namespace QDBusP2PExample;

auto main(int argc, char **argv) -> int
{
        QCoreApplication app(argc, argv);

        initQCoreApplication();

        Q_ASSERT(QMetaObject::invokeMethod(QCoreApplication::instance(), []() {
                auto server = new Server(QCoreApplication::instance());
                auto adaptor = new ServerAdaptor(server);
                Q_UNUSED(adaptor);

                auto address = getPeerToPeerSocketAddress();
                auto dirname = QDir::root().relativeFilePath(
                        QFileInfo(address).absolutePath());

                if (!QDir::root().exists(dirname) &&
                    !QDir::root().mkpath(dirname)) {
                        qCritical().noquote()
                                << QString("mkpath \"%1\" failed.")
                                           .arg("/" + dirname);
                        QCoreApplication::exit(-1);
                        return;
                }

                auto qDBusServer = new QDBusServer(
                        "unix:path=" + address, QCoreApplication::instance());
                if (!qDBusServer->isConnected()) {
                        qCritical() << "QDBusServer not connected:"
                                    << qDBusServer->lastError();
                        QCoreApplication::exit(-1);
                }
                QObject::connect(
                        QCoreApplication::instance(),
                        &QCoreApplication::aboutToQuit, [address]() {
                                if (QDir::root().remove(
                                            QDir::root().relativeFilePath(
                                                    address))) {
                                        return;
                                }

                                qCritical().noquote()
                                        << QString("remove \"%1\" failed.")
                                                   .arg(address);
                        });

                QList<QDBusConnection> connections;
                QObject::connect(
                        qDBusServer, &QDBusServer::newConnection,
                        [server](QDBusConnection conn) noexcept {
                                auto res = registerDBusObject(
                                        conn,
                                        "/io/github/blackdesk/QDBusP2PExample/Server",
                                        server);
                                if (!res.has_value()) {
                                        qCritical() << res.error().what();
                                        return;
                                }
                                QObject::connect(
                                        QCoreApplication::instance(),
                                        &QCoreApplication::aboutToQuit,
                                        [conn]() {
                                                unregisterDBusObject(
                                                        conn,
                                                        "/io/github/blackdesk/QDBusP2PExample/Server");
                                        });
                        });
        }));

        return QCoreApplication::exec();
}
