#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "Serializable.h"
#include <QString>

class Serializer {
public:
    virtual ~Serializer() = default;

    virtual void save(const Serializable&, const QString&, const bool, const bool);

    virtual void load(Serializable&, const QString&, bool&, bool&);
};

#endif // SERIALIZER_H
