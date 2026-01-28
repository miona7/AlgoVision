#include "Serializer.h"

void Serializer::save(const Serializable& s, const QString& filePath, bool isWeighted,
                      bool isDirected) {

    if(filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);
    if(!file.open(QFile::WriteOnly)) {
        return;
    }

    const QVariant payload = s.toVariant();

    QVariantMap root;
    root["schemaVersion"] = 1;
    root["isWeighted"]    = isWeighted;
    root["isDirected"]    = isDirected;
    root["graph"]         = payload;

    const auto jsonDoc = QJsonDocument::fromVariant(root);
    file.write(jsonDoc.toJson());
}

void Serializer::load(Serializable& serializable, const QVariant& data) {
    QVariantMap root = data.toMap();
    if(root.isEmpty()) {
        return; // prazno ili nevalidno
    }

    serializable.fromVariant(root);
}
