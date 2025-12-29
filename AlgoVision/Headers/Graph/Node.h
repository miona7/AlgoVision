#ifndef NODE_H
#define NODE_H

#include <QPointF>
#include <QString>
#include <QVector>

#include <ostream>
#include <string>

class Node {
public:
    Node(unsigned);
    ~Node() = default;

    unsigned getId() const;

    friend std::ostream& operator<<(std::ostream&, const Node&);

private:
    unsigned m_id;
};

std::ostream& operator<<(std::ostream&, const Node&);

#endif // NODE_H
