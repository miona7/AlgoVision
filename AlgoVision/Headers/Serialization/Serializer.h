#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <QString>
#include <QVariant>
#include <QVariantMap>
#include <QFile>
#include <QJsonDocument>

#include "Serializable.h"

class Serializer {
public:
    virtual ~Serializer() = default;

    virtual void save(const Serializable&, const QString&, bool, bool);
    virtual void load(Serializable&, const QVariant&);
};

#endif // SERIALIZER_H
