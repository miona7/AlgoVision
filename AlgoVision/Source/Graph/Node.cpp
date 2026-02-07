#include "Node.h"

Node::Node(unsigned id, double x, double y)
    : m_id(id), m_name(QString::number(id)), m_position {x, y} {
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

QString Node::getName() const {
    return m_name;
}

int Node::getComponentColor() const {
    return m_componentColor;
}

void Node::setPosition(double x, double y) {
    m_position = {x, y};
}

void Node::setState(NodeState state) {
    if(m_state == state) {
        return;
    }
    m_state = state;
    notifyObservers(*this); // obavestavamo sve posmatrace da se stanje promenilo
}

void Node::setName(const QString& newName) {
    m_name = newName;
}

void Node::setComponentColor(int colorId) {
    m_componentColor = colorId;
}
