#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>

class Node {
public:
    explicit Node(unsigned, double x = 0.0, double y = 0.0);
    ~Node() = default;

    unsigned getId() const;
    const std::pair<double, double>& getPosition() const;

    void setPosition(double, double);

    friend std::ostream& operator<<(std::ostream&, const Node&);

private:
    unsigned m_id;
    std::pair<double, double> m_position; // x, y koordinata cvora
};

std::ostream& operator<<(std::ostream&, const Node&);

#endif // NODE_H
