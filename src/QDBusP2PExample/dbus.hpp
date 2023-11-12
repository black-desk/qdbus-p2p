#pragma once

#include <QDBusConnection>
#include <QDBusError>
#include <QDebug>
#include <QStandardPaths>
#include <tl/expected.hpp>

namespace QDBusP2PExample
{
[[nodiscard]]
inline auto registerDBusObject(QDBusConnection conn, const QString &path,
                               QObject *obj) noexcept
        -> tl::expected<void, std::exception>
{
        if (conn.registerObject(path, obj)) {
                qDebug() << "register object" << path << "on" << conn.name();
                return {};
        }

        return tl::unexpected(
                std::runtime_error("register dbus object: object existed."));
}

inline void unregisterDBusObject(QDBusConnection conn,
                                 const QString &path) noexcept
{
        conn.unregisterObject(path);
        qDebug() << "unregister object" << path << "on" << conn.name();
}

[[nodiscard]]
inline auto registerDBusService(QDBusConnection conn,
                                const QString &serviceName) noexcept
        -> tl::expected<void, std::exception>
{
        if (conn.registerService(serviceName)) {
                qDebug() << "register dbus name" << serviceName;
                return {};
        }

        if (conn.lastError().isValid()) {
                return tl::unexpected(std::runtime_error(
                        QString("register dbus service: %1: %2")
                                .arg(conn.lastError().name(),
                                     conn.lastError().message())
                                .toStdString()));
        }

        return tl::unexpected(std::runtime_error(
                "register dbus service: service name existed."));
}

[[nodiscard]]
inline auto unregisterDBusService(QDBusConnection conn,
                                  const QString &serviceName) noexcept
        -> tl::expected<void, std::exception>
{
        if (conn.unregisterService(serviceName)) {
                qDebug() << "unregister dbus name" << serviceName;
                return {};
        }

        if (conn.lastError().isValid()) {
                return tl::unexpected(std::runtime_error(
                        QString("unregister dbus service: %1: %2")
                                .arg(conn.lastError().name(),
                                     conn.lastError().message())
                                .toStdString()));
        }

        return tl::unexpected(
                std::runtime_error("unregister dbus service: unknown."));
}

[[nodiscard]]
inline auto getPeerToPeerSocketAddress() noexcept
{
        auto runtimeDir = QStandardPaths::writableLocation(
                QStandardPaths::RuntimeLocation);

        return QString("%1/blackdesk/QDBusP2PExample/server").arg(runtimeDir);
}

}
