#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <QFile>
#include <QJsonDocument>
#include <QString>
#include <QVariantMap>

#include "Serializable.h"

class Serializer {
public:
    virtual ~Serializer() = default;

    virtual void save(const Serializable&, const QString&, bool, bool);

    virtual void load(Serializable&, const QString&, bool&, bool&);
};

#endif // SERIALIZER_H
