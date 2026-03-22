#include "AlgorithmWorker.h"

AlgorithmWorker::AlgorithmWorker(const QString& algorithm, const std::shared_ptr<Graph> graph,
                                 unsigned start, unsigned end, QObject* parent)
    : QThread(parent), m_algorithm(algorithm), m_graph(graph), m_start(start), m_end(end) {
}

void AlgorithmWorker::run() {
    std::unique_ptr<Algorithm> alg;

    if(m_algorithm == "A* (Euclidean heuristic)") {
        alg = std::make_unique<AStar>(m_graph);
    } else if(m_algorithm == "BFS") {
        alg = std::make_unique<BFS>(m_graph);
    } else if(m_algorithm == "Bellman-Ford") {
        alg = std::make_unique<BellmanFord>(m_graph);
    } else if(m_algorithm == "DFS") {
        alg = std::make_unique<DFS>(m_graph);
    } else if(m_algorithm == "Dijkstra") {
        alg = std::make_unique<Dijkstra>(m_graph);
    } else if(m_algorithm == "Prim") {
        alg = std::make_unique<Prim>(m_graph);
    } else if(m_algorithm == "Floyd-Warshall") {
        alg = std::make_unique<FloydWarshall>(m_graph);
    } else if(m_algorithm == "Tarjan") {
        alg = std::make_unique<Tarjan>(m_graph);
    } else if(m_algorithm == "Kahn") {
        alg = std::make_unique<Kahn>(m_graph);
    }

    if(alg != nullptr) {
        if(auto err = alg->execute()) {
            emit algorithmErrorOccurred(*err);
            return;
        }

        emit stepsReady(alg->getSteps());
        emit resultReady(alg->resultString());
    }
}
