#ifndef EDGE_H
#define EDGE_H

#include <iostream>

enum class EdgeState {
    Default,
    Examined,
    Relaxed,
    Selected,
    InPath
};

class Edge {
public:
    Edge(unsigned, unsigned, unsigned, int = 1);
    ~Edge() = default;

    unsigned getId() const;
    int      getWeight() const;
    EdgeState getState() const;
    unsigned startNode() const;
    unsigned endNode() const;

    void setWeight(int);
    void setstate(EdgeState);

    friend std::ostream& operator<<(std::ostream&, const Edge&);

private:
    int      m_weight;
    unsigned m_id;
    unsigned m_idStartNode;
    unsigned m_idEndNode;
    EdgeState m_state = EdgeState::Default;
};

std::ostream& operator<<(std::ostream&, const Edge&);

#endif // EDGE_H
