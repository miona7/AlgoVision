#include "Node.h"

Node::Node(unsigned id, double x, double y) : m_id(id), m_position {x, y} {
}

unsigned Node::getId() const {
    return m_id;
}

const std::pair<double, double>& Node::getPosition() const {
    return m_position;
}

NodeState Node::getState() const {
    return m_state;
}

void Node::setPosition(double x, double y) {
    m_position = {x, y};
}

void Node::setState(NodeState state) {
    m_state = state;
}

std::ostream& operator<<(std::ostream& s, const Node& n) {
    return s << "Node {id = " << n.m_id << "}" << std::endl;
}
