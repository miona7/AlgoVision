#include "LoadFile.h"

LoadFile::LoadFile(Serializer* serializer, Serializable* serializable, const QString& filePath,
                   QObject* parent)
    : QThread(parent), m_serializer(serializer), m_filePath(filePath),
      m_serializable(serializable) {
}

bool LoadFile::isWeighted() const {
    return m_isWeighted;
}

bool LoadFile::isDirected() const {
    return m_isDirected;
}

void LoadFile::run() {
    if(!m_serializer || !m_serializable) {
        return;
    }

    m_serializer->load(*m_serializable, m_filePath, m_isWeighted, m_isDirected);
}
