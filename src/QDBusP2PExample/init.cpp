#include "QDBusP2PExample/init.hpp"

#include <csignal>
#include <initializer_list>

#include <QCoreApplication>
#include <QDBusArgument>
#include <QDebug>
#include <QJsonObject>

#include "QDBusP2PExample/configure.hpp"

namespace QDBusP2PExample
{

namespace
{
void catchUnixSignals(std::initializer_list<int> quitSignals)
{
        auto handler = [](int sig) -> void {
                qInfo().noquote()
                        << QString("Quit the application by signal(%1).")
                                   .arg(sig);
                QCoreApplication::quit();
        };

        sigset_t blocking_mask;
        sigemptyset(&blocking_mask);
        for (auto sig : quitSignals)
                sigaddset(&blocking_mask, sig);

        struct sigaction sa {};
        sa.sa_handler = handler;
        sa.sa_mask = blocking_mask;
        sa.sa_flags = 0;

        for (auto sig : quitSignals)
                sigaction(sig, &sa, nullptr);
}
}

void initQCoreApplication() noexcept
{
        qDebug() << "version: "
                 << QString::fromLatin1(version.data(), version.size());

        catchUnixSignals({ SIGTERM, SIGQUIT, SIGINT, SIGHUP });
}

}
