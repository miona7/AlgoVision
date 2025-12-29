#include "Node.h"

#include <algorithm>

Node::Node(unsigned id) : m_id(id) {
}

unsigned Node::getId() const {
    return m_id;
}

std::ostream& operator<<(std::ostream& os, const Node& node) {
    return os << "Node {id = " << node.m_id << "}" << std::endl;
}
