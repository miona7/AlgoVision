#ifndef NODE_H
#define NODE_H

#include <QString>
#include <QVector>
#include <QPointF>

#include <string>
#include <ostream>

class Node {
public:
    Node();
    Node(const std::string&);
    Node(const Node&);
    ~Node();

    QString getName() const;
    int getId() const;

    QVector<Node*> getNeighbours() const;

    QPointF getPosition() const;

    void setName(const std::string&);
    void setName(const QString&);

    void translate(float, float);
    void setPosition(float, float);
    void setPosition(const QPointF&);

    bool operator==(const Node&) const;
    bool operator<(const Node&) const;

    void addNeighbour(Node*);
    void removeNeighbour(Node*);

    friend std::ostream& operator<<(std::ostream&, const Node&);

private:
    QString m_name;
    int m_id;

    static int nextId();

    QVector<Node*> m_neighbours;

    QPointF m_position;

};

std::ostream& operator<<(std::ostream&, const Node&);

#endif // NODE_H
