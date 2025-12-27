#include "Node.h"

#include <algorithm>

namespace nodeId {
    int m_nextId = 0;
}

int Node::nextId() {
    return nodeId::m_nextId++;
}

Node::Node()
    : m_name(""),
      m_id(nextId()),
      m_neighbours(),
      m_position(0.0, 0.0) {}

Node::Node(const Node& n)
    : m_name(n.m_name),
      m_id(n.m_id),
      m_neighbours(n.m_neighbours),
      m_position(n.m_position) {}

Node::Node(const std::string& s)
    : m_name(QString::fromStdString(s)),
      m_id(nextId()),
      m_neighbours(),
      m_position(0.0, 0.0) {}

Node::~Node() = default;

QString Node::getName() const {
    return m_name;
}

int Node::getId() const {
    return m_id;
}

QVector<Node*> Node::getNeighbours() const {
    return m_neighbours;
}

QPointF Node::getPosition() const {
    return m_position;
}

void Node::setPosition(float x, float y) {
    m_position = QPointF(x, y);
}

void Node::setPosition(const QPointF& p) {
    m_position = p;
}

void Node::setName(const std::string& name)
{
    m_name = QString::fromStdString(name);
}

void Node::setName(const QString& name)
{
    m_name = name;
}

void Node::translate(float dx, float dy) {
    m_position.setX(m_position.x() + dx);
    m_position.setY(m_position.y() + dy);
}

bool Node::operator==(const Node& n) const {
    return m_id == n.m_id;
}

void Node::addNeighbour(Node* node) {
    if (node == nullptr) {
        return;
    }

    auto it = std::find(m_neighbours.begin(), m_neighbours.end(), node);
    if (it != m_neighbours.end()) {
        return;
    }

    m_neighbours.push_back(node);
}

void Node::removeNeighbour(Node* node){
    if(node == nullptr){
        return;
    }

    for(auto it = m_neighbours.begin(); it != m_neighbours.end(); ){
        if(*it == node){
            it = m_neighbours.erase(it); // vraca pokazivac na sledeci nakon izbacivanja
        } else {
            ++it;
        }
    }
}

std::ostream& operator<<(std::ostream& os, const Node& node)
{
    return os << "{id = " << node.m_id
              << ", name = " << node.m_name.toStdString()
              << ", pos = (" << node.m_position.x() << ", " << node.m_position.y() << ")"
              << "}"
              << std::endl;
}
