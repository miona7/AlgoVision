#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "Serialization/Serializable.h"
#include <QString>

class Serializer
{
public:
    virtual ~Serializer() = default;

    virtual void save(const Serializable &s,
                      const QString &filePath,
                      const bool isWeighted,
                      const bool isDirected);

    virtual void load(Serializable &s,
                      const QString &filePath,
                      bool &isWeighted,
                      bool &isDirected);
};

#endif // SERIALIZER_H
