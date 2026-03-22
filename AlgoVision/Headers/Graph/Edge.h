#ifndef EDGE_H
#define EDGE_H

#include "Observable.h"

enum class EdgeState { Default, Examined, Relaxed, Selected };

class Edge : public Observable<Edge> {
public:
    Edge(unsigned, unsigned, unsigned, int = 1);
    ~Edge() = default;

    unsigned  getId() const;
    int       getWeight() const;
    EdgeState getState() const;
    unsigned  startNode() const;
    unsigned  endNode() const;

    void setWeight(int);
    void setState(EdgeState);

private:
    int       m_weight;
    unsigned  m_id;
    unsigned  m_idStartNode;
    unsigned  m_idEndNode;
    EdgeState m_state = EdgeState::Default;
};

#endif // EDGE_H
