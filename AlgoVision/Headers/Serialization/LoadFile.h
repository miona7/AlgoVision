#ifndef LOADFILE_H
#define LOADFILE_H

#include <QString>
#include <QThread>

#include "Serializable.h"
#include "Serializer.h"

class Serializer;
class Serializable;

class LoadFile : public QThread {
public:
    LoadFile(Serializer*, Serializable*, const QString&, QObject* = nullptr);

    void run() override;

    bool isWeighted() const;
    bool isDirected() const;

private:
    Serializer* m_serializer = nullptr;
    QString     m_filePath;

    Serializable* m_serializable = nullptr;

    bool m_isWeighted = false;
    bool m_isDirected = false;
};

#endif // LOADFILE_H
