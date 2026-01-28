#ifndef SAVEFILEWORKER_H
#define SAVEFILEWORKER_H

#include <QString>
#include <QThread>

#include "Serializable.h"
#include "Serializer.h"

class SaveFileWorker : public QThread {
public:
    SaveFileWorker(Serializer*, Serializable*, const QString&, bool, bool, QObject* = nullptr);

    void run() override;

private:
    Serializer* m_serializer = nullptr;
    QString     m_filePath;

    Serializable* m_serializable = nullptr;

    bool m_isWeighted = false;
    bool m_isDirected = false;
};

#endif // SAVEFILEWORKER_H
