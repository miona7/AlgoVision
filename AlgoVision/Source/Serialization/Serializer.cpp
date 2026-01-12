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

void Serializer::load(Serializable& s, const QString& filePath, bool& isWeighted,
                      bool& isDirected) {

    if(filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);
    if(!file.open(QFile::ReadOnly)) {
        return;
    }

    const auto        jsonDoc = QJsonDocument::fromJson(file.readAll());
    const QVariant    rootVar = jsonDoc.toVariant();
    const QVariantMap root    = rootVar.toMap();

    isWeighted = root.value("isWeighted").toBool();
    isDirected = root.value("isDirected").toBool();

    const QVariant graphVar = root.value("graph");
    if(!graphVar.isValid()) {
        return;
    }

    s.fromVariant(graphVar);
}
