#include "Node.h"

Node::Node(unsigned id, double x, double y) : m_id(id), m_position{x, y} {
}

unsigned Node::getId() const {
    return m_id;
}

const std::pair<double, double>& Node::getPosition() const {
    return m_position;
}

void Node::setPosition(double x, double y) {
    m_position = {x, y};
}

std::ostream& operator<<(std::ostream& s, const Node& n) {
    return s << "Node {id = " << n.m_id << "}" << std::endl;
}

