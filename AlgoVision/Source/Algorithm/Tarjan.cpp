#include "Tarjan.h"

bool Tarjan::checkConditions() const {
    if(m_graph == nullptr) {
        return false;
    }

    if(m_graph->size() == 0) {
        return false;
    }

    for(const auto& node: m_graph->getNodes()) {
        if(node == nullptr) {
            return false;
        }
    }

    return true;
}

void Tarjan::execute(Node* startNode, Node* endNode) {
    if(!checkConditions()) {
        throw std::runtime_error("Graph is not initialized or invalid!");
    }

    int numOfNodes = m_graph->size();
    m_incomingNumbering.resize(numOfNodes, -1);
    m_lowLink.resize(numOfNodes);
    m_onStack.resize(numOfNodes, false);
    m_components.resize(numOfNodes, -1);

    int component = 0;

    for(auto* node : m_graph->getNodes()) {
        int nodeId = node->getId();
        if(m_components[nodeId] == -1) {
            tarjan(nodeId, node, component);
        }
    }
}

void Tarjan::tarjan(int nodeId, Node* node, int& component) {
    m_incomingNumbering[nodeId] = m_lowLink[nodeId] = m_arrivalTime;
    m_arrivalTime++;
    m_tourOrder.push(nodeId);
    m_onStack[nodeId] = true;

    for(auto* neighbour : node->getNeighbours()) {
        int idNeighbour = neighbour->getId();
        if(m_incomingNumbering[idNeighbour] == -1) {
            tarjan(idNeighbour, neighbour, component);
            m_lowLink[nodeId] = std::min(m_lowLink[nodeId], m_lowLink[idNeighbour]);
        } else if(m_onStack[idNeighbour]) {
            m_lowLink[nodeId] = std::min(m_lowLink[nodeId], m_incomingNumbering[idNeighbour]);
        }
    }

    if(m_incomingNumbering[nodeId] == m_lowLink[nodeId]) {
        while(true) {
            int componentNodeId = m_tourOrder.top();
            m_tourOrder.pop();

            m_components[componentNodeId] = component;
            m_onStack[componentNodeId] = false;

            if(componentNodeId == nodeId) {
                break;
            }
        }
        component++;
    }
}
