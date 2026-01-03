#ifndef LOADFILE_H
#define LOADFILE_H

#include <QThread>
#include <QString>

class Serializer;
class Serializable;

class LoadFile : public QThread
{
public:
    LoadFile(Serializer *serializer,
             Serializable *serializable,
             const QString &filePath,
             QObject *parent = nullptr);

    void run() override;

    bool isWeighted() const;
    bool isDirected() const;

private:
    Serializer *m_serializer = nullptr;
    QString m_filePath;

    Serializable *m_serializable = nullptr;

    bool m_isWeighted = false;
    bool m_isDirected = false;
};

#endif // LOADFILE_H
