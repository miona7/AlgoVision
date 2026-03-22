#ifndef LOADFILEWORKER_H
#define LOADFILEWORKER_H

#include <QObject>
#include <QThread>
#include <QString>

#include "Serializable.h"
#include "Serializer.h"

class LoadFileWorker : public QThread {
    Q_OBJECT
public:
    LoadFileWorker(Serializer*, const QString&, QObject* = nullptr);

signals:
    void loaded(const QVariant&, bool, bool, const QString&);
    void failed(const QString&);

protected:
    void run() override;

private:
    Serializer* m_serializer = nullptr;
    QString     m_filePath;
};

#endif // LOADFILEWORKER_H
