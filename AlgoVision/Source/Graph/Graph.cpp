#include "Graph.h"

Node* Graph::addNode(double x, double y) {
    unsigned id = m_nodeId++;
    addNode(id, x, y);
    return getNode(id);
}

void Graph::addNode(unsigned id, double x, double y) {
    if(m_nodes.find(id) != m_nodes.end()) {
        return; // cvor vec postoji
    }
    ++m_numOfNodes;
    m_adjacencyList[id] = {};
    m_nodes.emplace(id, Node(id, x, y));
}

void Graph::removeNode(unsigned id) {
    if(m_nodes.find(id) == m_nodes.end()) {
        return; // cvor ne postoji
    }

    // sve grane povezane sa cvorom
    std::set<unsigned> edgesToRemove;

    // izlazne grane
    if(m_adjacencyList.find(id) != m_adjacencyList.end()) {
        for(const auto& [edgeId, _]: m_adjacencyList[id]) {
            edgesToRemove.insert(edgeId);
        }
    }

    // ulazne grane
    for(const auto& [_, neighbors]: m_adjacencyList) {
        for(const auto& [edgeId, to]: neighbors) {
            if(to == id) {
                edgesToRemove.insert(edgeId);
            }
        }
    }

    // obrisi sve grane
    for(unsigned edgeId: edgesToRemove) {
        removeEdge(edgeId);
    }

    // obrisi cvor
    m_adjacencyList.erase(id);
    m_nodes.erase(id);
    --m_numOfNodes;
}

void Graph::clear() {
    m_numOfNodes = 0;
    m_numOfEdges = 0;
    m_adjacencyList.clear();
    m_nodes.clear();
    m_edges.clear();
}

// razlikuje se od obicnog dodavanja grane
void Graph::addEdgeSerialized(unsigned edgeId, unsigned from, unsigned to, int w) {
    if(m_nodes.find(from) == m_nodes.end() || m_nodes.find(to) == m_nodes.end()) {
        return;
    }

    m_edges.emplace(edgeId, Edge(edgeId, from, to, w));

    m_adjacencyList[from][edgeId] = to;
    if(!isDirected()) {
        m_adjacencyList[to][edgeId] = from;
    }

    if(edgeId > m_numOfEdges) {
        m_numOfEdges = edgeId;
    }
}

QVariant Graph::toVariant() const {
    QVariantList nodes;
    nodes.reserve(static_cast<int>(m_nodes.size()));

    for(const auto& [id, node]: m_nodes) {
        const auto& pos = node.getPosition();

        QVariantMap n;
        n["id"] = id;
        n["x"]  = pos.first;
        n["y"]  = pos.second;

        nodes.push_back(n);
    }

    QVariantList edges;
    edges.reserve(static_cast<int>(m_edges.size()));

    for(const auto& [id, edge]: m_edges) {
        QVariantMap e;
        e["id"]     = edge.getId();
        e["from"]   = edge.startNode();
        e["to"]     = edge.endNode();
        e["weight"] = edge.getWeight();

        edges.push_back(e);
    }

    QVariantMap graph;
    graph["nodes"] = nodes;
    graph["edges"] = edges;
    return graph;
}

void Graph::fromVariant(const QVariant& variant) {
    const QVariantMap graph = variant.toMap();
    if(graph.isEmpty()) {
        return;
    }

    clear();

    const QVariantList nodes = graph.value("nodes").toList();
    for(const QVariant& v: nodes) {
        const QVariantMap n  = v.toMap();
        const unsigned    id = n.value("id").toUInt();
        const double      x  = n.value("x").toDouble();
        const double      y  = n.value("y").toDouble();
        addNode(id, x, y);
    }

    const QVariantList edges = graph.value("edges").toList();
    for(const QVariant& v: edges) {
        const QVariantMap e = v.toMap();

        const unsigned edgeId = e.value("id").toUInt();
        const unsigned from   = e.value("from").toUInt();
        const unsigned to     = e.value("to").toUInt();
        const int      w      = e.value("weight", 1).toInt();

        addEdgeSerialized(edgeId, from, to, w);
    }
}

std::map<unsigned, std::map<unsigned, unsigned>> Graph::getAdjacencyList() const {
    return m_adjacencyList;
}

std::map<unsigned, Node> Graph::getNodes() const {
    return m_nodes;
}

std::map<unsigned, Edge> Graph::getEdges() const {
    return m_edges;
}

Node* Graph::getNode(unsigned id) {
    auto it = m_nodes.find(id);
    return (it != m_nodes.end()) ? &it->second : nullptr;
}

const Node* Graph::getNode(unsigned id) const {
    auto it = m_nodes.find(id);
    return (it != m_nodes.end()) ? &it->second : nullptr;
}

Edge* Graph::getEdge(unsigned from, unsigned to) {
    for(auto& [_, edge]: m_edges) {
        if(edge.startNode() == from && edge.endNode() == to) {
            return &edge;
        }
        if(!isDirected() && edge.startNode() == to && edge.endNode() == from) {
            return &edge;
        }
    }
    return nullptr;
}

const Edge* Graph::getEdge(unsigned from, unsigned to) const {
    for(auto& [_, edge]: m_edges) {
        if(edge.startNode() == from && edge.endNode() == to) {
            return &edge;
        }
        if(!isDirected() && edge.startNode() == to && edge.endNode() == from) {
            return &edge;
        }
    }
    return nullptr;
}
