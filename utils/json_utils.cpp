#include "json_utils.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch"

QJsonDocument JsonUtils::hashmapToJson(const QHash<QString, QVariant>& hashmap)
{
    QJsonDocument doc;

    QJsonObject obj;

    for (QHash<QString, QVariant>::const_iterator it = hashmap.constBegin(); it != hashmap.constEnd(); ++it)
    {
        typedef QVariant::Type T;

        switch (it.value().type())
        {
            case T::Bool:
                obj.insert(it.key(), it.value().toBool());
                break;
            case T::Int:
            case T::UInt:
            case T::LongLong:
            case T::ULongLong:
                obj.insert(it.key(), it.value().toLongLong());
                break;
            case T::Double:
                obj.insert(it.key(), it.value().toDouble());
                break;
            case T::List:
                obj.insert(it.key(), QJsonArray::fromVariantList(it.value().toList()));
                break;
            case T::String:
            default:
                obj.insert(it.key(), it.value().toString());
                break;
        }
    }

    doc.setObject(obj);
    return doc;
}

#pragma GCC diagnostic pop

QHash<QString, QVariant> JsonUtils::jsonToHashmap(const QJsonDocument& doc)
{
    QHash<QString, QVariant> hashmap;

    if (doc.isNull() || doc.isEmpty())
        return hashmap;

    QJsonObject obj = doc.object();

    for (QJsonObject::const_iterator it = obj.constBegin(); it != obj.constEnd(); ++it)
    {
        hashmap.insert(it.key(), it.value());
    }

    return hashmap;
}


QString JsonUtils::jsonToString(const QJsonDocument& doc)
{
    return doc.toJson(QJsonDocument::Compact);
}