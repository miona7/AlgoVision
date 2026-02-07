#include "LoadFileWorker.h"

LoadFileWorker::LoadFileWorker(Serializer* serializer, const QString& filePath, QObject* parent)
    : QThread(parent), m_serializer(serializer), m_filePath(filePath) {
}

void LoadFileWorker::run() {
    if(m_serializer == nullptr) {
        emit failed("Serializer not initialized");
        return;
    }

    QFile file(m_filePath);
    if(!file.open(QFile::ReadOnly)) {
        emit failed("Cannot open file: " + m_filePath);
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QVariantMap root = doc.toVariant().toMap();
    if(root.isEmpty()) {
        emit failed("Empty or invalid file");
        return;
    }

    bool weighted = root.value("isWeighted").toBool();
    bool directed = root.value("isDirected").toBool();

    // get only "graph"
    QVariant graphData = root.value("graph");
    if(!graphData.isValid()) {
        emit failed("No graph data in file");
        return;
    }

    emit loaded(graphData, weighted, directed, m_filePath); // send only QVariant
}
