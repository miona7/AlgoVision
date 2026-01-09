#include "Edge.h"

Edge::Edge(unsigned id, unsigned idStartNode, unsigned idEndNode, int weight)
    : m_id(id), m_idStartNode(idStartNode), m_idEndNode(idEndNode), m_weight(weight) {
}

unsigned Edge::getId() const {
    return m_id;
}

int Edge::getWeight() const {
    return m_weight;
}

EdgeState Edge::getState() const {
    return m_state;
}

unsigned Edge::startNode() const {
    return m_idStartNode;
}

unsigned Edge::endNode() const {
    return m_idEndNode;
}

void Edge::setWeight(int w) {
    m_weight = w;
}

void Edge::setState(EdgeState state) {
    m_state = state;
}

std::ostream& operator<<(std::ostream& s, const Edge& e) {
    return s << "Edge {id = " << e.m_id << ", from node " << e.m_idStartNode << " to node "
             << e.m_idEndNode << ", with weight = " << e.m_weight << "}" << std::endl;
}
