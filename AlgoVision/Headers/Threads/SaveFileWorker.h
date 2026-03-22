#ifndef SAVEFILEWORKER_H
#define SAVEFILEWORKER_H

#include <QObject>
#include <QString>
#include <QThread>

#include "Serializer.h"
#include "Serializable.h"

class SaveFileWorker : public QThread {
    Q_OBJECT
public:
    SaveFileWorker(Serializer*, Serializable*, const QString&, bool, bool, QObject* = nullptr);

signals:
    void finished();
    void failed(const QString&);

protected:
    void run() override;

private:
    Serializer* m_serializer{nullptr};
    QString     m_filePath;

    Serializable* m_serializable{nullptr};

    bool m_isWeighted{false};
    bool m_isDirected{false};
};

#endif // SAVEFILEWORKER_H
