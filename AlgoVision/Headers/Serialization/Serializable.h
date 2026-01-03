#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <QVariant>

class Serializable {
public:
    virtual ~Serializable() = default;

    virtual QVariant toVariant() const            = 0;
    virtual void     fromVariant(const QVariant&) = 0;
};

#endif // SERIALIZABLE_H
