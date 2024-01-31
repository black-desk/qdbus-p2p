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

        auto ret = QMetaObject::invokeMethod(QCoreApplication::instance(), []() {
                auto server = // NOLINT
                        new Server(QCoreApplication::instance());
                auto adaptor = new ServerAdaptor(server); // NOLINT
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

                auto qDBusServer = new QDBusServer( // NOLINT
                        "unix:path=" + address, QCoreApplication::instance());
                if (!qDBusServer->isConnected()) {
                        qCritical() << "QDBusServer not connected:"
                                    << qDBusServer->lastError();
                        QCoreApplication::exit(-1);
                }

                // NOTE:
                // When other user connect to this message bus,
                // that connection will be closed immediately in libdbus
                // if we do not allow anonymous auth.
                // I have no idea about what is going on in that library.
                qDBusServer->setAnonymousAuthenticationAllowed(true);

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
                        [server](const QDBusConnection &conn) noexcept {
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
        });

        Q_ASSERT(ret);

        return QCoreApplication::exec();
}
