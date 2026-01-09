#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>
#include <string>

#include "Observable.h"

enum class NodeState {
    Default,
    Active,  // InProcess -> trenutno se obradjuje
    Visited, // posecen -> obradjen
    InPath,
    TopologicalOrder,
    AssignedComponent
};

class Node : public Observable<Node> {
public:
    explicit Node(unsigned, double = 0.0, double = 0.0);
    ~Node() = default;

    unsigned                         getId() const;
    const std::pair<double, double>& getPosition() const;
    NodeState                        getState() const;

    void setPosition(double, double);
    void setState(NodeState);

    friend std::ostream& operator<<(std::ostream&, const Node&);

private:
    unsigned                  m_id;
    std::pair<double, double> m_position; // x, y koordinata cvora
    NodeState                 m_state = NodeState::Default;
};

std::ostream& operator<<(std::ostream&, const Node&);

#endif // NODE_H
