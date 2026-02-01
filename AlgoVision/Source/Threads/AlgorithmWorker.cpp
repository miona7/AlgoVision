#include "AlgorithmWorker.h"

AlgorithmWorker::AlgorithmWorker(const QString& algorithm, const std::shared_ptr<Graph> graph,
                                 unsigned start, unsigned end)
    : m_algorithm(algorithm), m_graph(graph), m_start(start), m_end(end) {
}

void AlgorithmWorker::run() {
    std::vector<AlgorithmStep> steps;
    Algorithm*                 algo = nullptr;

    if(m_algorithm == "A* (Euclidean heuristic)") {
        algo = new AStar(m_graph);
        algo->execute(m_start, m_end);
    } else if(m_algorithm == "BFS") {
        algo = new BFS(m_graph);
        algo->execute(m_start);
    } else if(m_algorithm == "Bellman-Ford") {
        algo = new BellmanFord(m_graph);
        algo->execute(m_start);
    } else if(m_algorithm == "DFS") {
        algo = new DFS(m_graph);
        algo->execute(m_start);
    } else if(m_algorithm == "Dijkstra") {
        algo = new Dijkstra(m_graph);
        algo->execute(m_start);
    } else if(m_algorithm == "Prim") {
        algo = new Prim(m_graph);
        algo->execute();
    } else if(m_algorithm == "Floyd-Warshall") {
        algo = new FloydWarshall(m_graph);
        algo->execute(m_start);
    } else if(m_algorithm == "Tarjan") {
        algo = new Tarjan(m_graph);
        algo->execute();
    } else if(m_algorithm == "Kahn") {
        algo = new Kahn(m_graph);
        algo->execute();
    }

    if(algo != nullptr) {
        steps = algo->getSteps();

        QString result = algo->resultString();

        qDebug() << "ALGO:" << m_algorithm;
        qDebug() << "RESULT STRING:" << result;

        emit stepsReady(steps);
        emit resultReady(result);

        delete algo;
    }
}
