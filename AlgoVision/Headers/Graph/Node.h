#ifndef NODE_H
#define NODE_H

#include <QString>

#include "Observable.h"

enum class NodeState {
    Default,
    Active,
    Visited,
    UpdatedDistance,
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
    QString                          getName() const;
    int                              getComponentColor() const;

    void setPosition(double, double);
    void setState(NodeState);
    void setName(const QString&);
    void setComponentColor(int);

private:
    unsigned                  m_id;
    QString                   m_name;
    std::pair<double, double> m_position; // node coordinates
    NodeState                 m_state{NodeState::Default};
    int                       m_componentColor{-1};
};

#endif // NODE_H
