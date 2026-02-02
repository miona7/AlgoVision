#include "AlgorithmWorker.h"

AlgorithmWorker::AlgorithmWorker(const QString& algorithm, const std::shared_ptr<Graph> graph,
                                 unsigned start, unsigned end)
    : m_algorithm(algorithm), m_graph(graph), m_start(start), m_end(end) {
}

void AlgorithmWorker::run() {
    std::vector<AlgorithmStep> steps;
    if(m_algorithm == "A*") {
        AStar astar(m_graph);

        if(auto err = astar.execute(m_start, m_end)) {
            emit algorithmErrorOccurred(*err);
            return; // nema stepsReady jer se javila greska - prekidamo run - dalje preuzima
                    // kontroler
        }

        steps = astar.getSteps();
    }
    if(m_algorithm == "BFS") {
        BFS bfs(m_graph);

        if(auto err = bfs.execute(m_start)) {
            emit algorithmErrorOccurred(*err);
            return;
        }

        steps = bfs.getSteps();
    }
    if(m_algorithm == "Bellman-Ford") {
        BellmanFord bf(m_graph);

        if(auto err = bf.execute(m_start)) {
            emit algorithmErrorOccurred(*err);
            return;
        }

        steps = bf.getSteps();
    }
    if(m_algorithm == "DFS") {
        DFS dfs(m_graph);

        if(auto err = dfs.execute(m_start)) {
            emit algorithmErrorOccurred(*err);
            return;
        }

        steps = dfs.getSteps();
    }
    if(m_algorithm == "Dijkstra") {
        Dijkstra dijkstra(m_graph);

        if(auto err = dijkstra.execute(m_start)) {
            emit algorithmErrorOccurred(*err);
            return;
        }

        steps = dijkstra.getSteps();
    }
    if(m_algorithm == "Prim") {
        Prim prim(m_graph);

        if(auto err = prim.execute()) {
            emit algorithmErrorOccurred(*err);
            return;
        }

        steps = prim.getSteps();
    }
    if(m_algorithm == "Floyd-Warshall") {
        FloydWarshall fw(m_graph);

        if(auto err = fw.execute(m_start)) {
            emit algorithmErrorOccurred(*err);
            return;
        }

        steps = fw.getSteps();
    }
    if(m_algorithm == "Tarjan") {
        Tarjan tarjan(m_graph);

        if(auto err = tarjan.execute()) {
            emit algorithmErrorOccurred(*err);
            return;
        }

        steps = tarjan.getSteps();
    }
    if(m_algorithm == "Kahn") {
        Kahn kahn(m_graph);

        if(auto err = kahn.execute()) {
            emit algorithmErrorOccurred(*err);
            return;
        }

        steps = kahn.getSteps();
    }

    emit stepsReady(steps); // signal da su koraci spremni, u isto vreme ih i saljemo
}
