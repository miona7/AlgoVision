#include "SaveFileWorker.h"

SaveFileWorker::SaveFileWorker(Serializer* serializer, Serializable* serializable, const QString& filePath,
                   bool isWeighted, bool isDirected, QObject* parent)
    : QThread(parent), m_serializer(serializer), m_filePath(filePath), m_serializable(serializable),
      m_isWeighted(isWeighted), m_isDirected(isDirected) {
}

void SaveFileWorker::run() {
    if(m_serializer == nullptr || m_serializable == nullptr) {
        return;
    }

    m_serializer->save(*m_serializable, m_filePath, m_isWeighted, m_isDirected);
}
