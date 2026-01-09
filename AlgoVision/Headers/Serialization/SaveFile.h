#ifndef SAVEFILE_H
#define SAVEFILE_H

#include <QString>
#include <QThread>

#include "Serializable.h"
#include "Serializer.h"

class SaveFile : public QThread {
public:
    SaveFile(Serializer*, Serializable*, const QString&, bool, bool, QObject* = nullptr);

    void run() override;

private:
    Serializer* m_serializer = nullptr;
    QString     m_filePath;

    Serializable* m_serializable = nullptr;

    bool m_isWeighted = false;
    bool m_isDirected = false;
};

#endif // SAVEFILE_H
