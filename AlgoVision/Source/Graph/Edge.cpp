#include "Edge.h"

Edge::Edge(Node *u, Node *v, bool directed, int weight)
    : m_connectedNodes({u, v}), m_directed(directed), m_weight(weight) {
}

Edge::Edge(const Edge &e)
    : Edge(e.startNode(), e.endNode(), e.isDirected(), e.getWeight()) {
}

Edge::~Edge() = default;

bool Edge::isDirected() const {
    return m_directed;
}

int Edge::getWeight() const {
    return m_weight;
}

Node* Edge::startNode() const {
    return m_connectedNodes.first;
}

Node* Edge::endNode() const {
    return m_connectedNodes.second;
}

void Edge::setWeight(int w) {
    m_weight = w;
}

std::ostream &operator<<(std::ostream & s, const Edge &e) {
    return s << "Edge from " << e.startNode()->getName().toStdString() << " to "
            << e.endNode()->getName().toStdString() << ", weight = " << e.getWeight() << std::endl;
}
