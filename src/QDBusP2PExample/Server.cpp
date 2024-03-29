#include "QDBusP2PExample/Server.hpp"

#include <QDebug>
#include <QJsonObject>
#include <qdbusargument.h>

namespace QDBusP2PExample
{
namespace
{
static QVariant decodeQDBusArgument(const QVariant &v)
{
        if (!v.canConvert<QDBusArgument>()) {
                return v;
        }

        // we use QJsonValue to resolve all data type in DConfigInfo class, so it's type is equal QJsonValue::Type,
        // now we parse Map and Array type to QVariant explicitly.
        const QDBusArgument &complexType = v.value<QDBusArgument>();
        switch (complexType.currentType()) {
        case QDBusArgument::MapType: {
                QVariantMap list;
                complexType >> list;
                QVariantMap res;
                for (auto iter = list.begin(); iter != list.end(); iter++) {
                        res[iter.key()] = decodeQDBusArgument(iter.value());
                }
                return res;
        }
        case QDBusArgument::ArrayType: {
                QVariantList list;
                complexType >> list;
                QVariantList res;
                res.reserve(list.size());
                for (const auto &item : qAsConst(list)) {
                        res << decodeQDBusArgument(item);
                }
                return res;
        }
        default:
                Q_ASSERT(false);
                return QVariant{};
        }
}

static auto fromVariantMap(const QVariantMap &map) -> QJsonObject
{
        QVariantMap newMap;
        for (auto it = map.constBegin(); it != map.end(); it++) {
                newMap.insert(it.key(), decodeQDBusArgument(it.value()));
        }
        return QJsonObject::fromVariantMap(newMap);
}
}

QVariantMap Server::TestMethod(const QVariantMap &parameters)
{
        auto object = fromVariantMap(parameters);
        qInfo() << "method called" << object;
        this->TestSignal();
        return object.toVariantMap();
}
}
