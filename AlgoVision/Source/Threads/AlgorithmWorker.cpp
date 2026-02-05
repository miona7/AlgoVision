#include "AlgorithmWorker.h"

AlgorithmWorker::AlgorithmWorker(const QString& algorithm, const std::shared_ptr<Graph> graph,
                                 unsigned start, unsigned end)
    : m_algorithm(algorithm), m_graph(graph), m_start(start), m_end(end) {
}

void AlgorithmWorker::run() {
    std::vector<AlgorithmStep> steps;
    QString                    result = "";

    if(m_algorithm == "A* (Euclidean heuristic)") {
        AStar astar(m_graph);

        if(auto err = astar.execute(m_start, m_end)) {
            emit algorithmErrorOccurred(*err);
            return;
        }

        steps  = astar.getSteps();
        result = astar.resultString();
    }
    if(m_algorithm == "BFS") {
        BFS bfs(m_graph);

        if(auto err = bfs.execute(m_start)) {
            emit algorithmErrorOccurred(*err);
            return;
        }

        steps  = bfs.getSteps();
        result = bfs.resultString();
    }
    if(m_algorithm == "Bellman-Ford") {
        BellmanFord bf(m_graph);

        if(auto err = bf.execute(m_start)) {
            emit algorithmErrorOccurred(*err);
            return;
        }

        steps  = bf.getSteps();
        result = bf.resultString();
    }
    if(m_algorithm == "DFS") {
        DFS dfs(m_graph);

        if(auto err = dfs.execute(m_start)) {
            emit algorithmErrorOccurred(*err);
            return;
        }

        steps  = dfs.getSteps();
        result = dfs.resultString();
    }
    if(m_algorithm == "Dijkstra") {
        Dijkstra dijkstra(m_graph);

        if(auto err = dijkstra.execute(m_start)) {
            emit algorithmErrorOccurred(*err);
            return;
        }

        steps  = dijkstra.getSteps();
        result = dijkstra.resultString();
    }
    if(m_algorithm == "Prim") {
        Prim prim(m_graph);

        if(auto err = prim.execute()) {
            emit algorithmErrorOccurred(*err);
            return;
        }

        steps  = prim.getSteps();
        result = prim.resultString();
    }
    if(m_algorithm == "Floyd-Warshall") {
        FloydWarshall fw(m_graph);

        if(auto err = fw.execute(m_start)) {
            emit algorithmErrorOccurred(*err);
            return;
        }

        steps  = fw.getSteps();
        result = fw.resultString();
    }
    if(m_algorithm == "Tarjan") {
        Tarjan tarjan(m_graph);

        if(auto err = tarjan.execute()) {
            emit algorithmErrorOccurred(*err);
            return;
        }

        steps  = tarjan.getSteps();
        result = tarjan.resultString();
    }
    if(m_algorithm == "Kahn") {
        Kahn kahn(m_graph);

        if(auto err = kahn.execute()) {
            emit algorithmErrorOccurred(*err);
            return;
        }

        steps  = kahn.getSteps();
        result = kahn.resultString();
    }

    emit stepsReady(steps);
    emit resultReady(result);
}
