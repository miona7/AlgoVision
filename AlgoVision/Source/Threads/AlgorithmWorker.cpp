#include "AlgorithmWorker.h"

AlgorithmWorker::AlgorithmWorker(const QString& algorithm, std::shared_ptr<Graph> graph, unsigned start, unsigned end)
    : m_algorithm(algorithm), m_graph(graph), m_start(start), m_end(end) {}

void AlgorithmWorker::run() {
    std::vector<AlgorithmStep> steps;
    if(m_algorithm == "A*") {
        AStar astar(m_graph);
        astar.execute(m_start, m_end);
        steps = astar.getSteps();
    }
    if(m_algorithm == "BFS") {
        BFS bfs(m_graph);
        bfs.execute(m_start);
        steps = bfs.getSteps();
    }
    if(m_algorithm == "Bellman-Ford") {
        BellmanFord bf(m_graph);
        bf.execute(m_start);
        steps = bf.getSteps();
    }
    if(m_algorithm == "DFS") {
        DFS dfs(m_graph);
        dfs.execute(m_start);
        steps = dfs.getSteps();
    }
    if(m_algorithm == "Dijkstra") {
        Dijkstra dijkstra(m_graph);
        dijkstra.execute(m_start);
        steps = dijkstra.getSteps();
    }
    if(m_algorithm == "Prim") {
        Prim prim(m_graph);
        prim.execute();
        steps = prim.getSteps();
    }
    if(m_algorithm == "Floyd-Warshall") {
        FloydWarshall fw(m_graph);
        fw.execute(m_start);
        steps = fw.getSteps();
    }
    if(m_algorithm == "Tarjan") {
        Tarjan tarjan(m_graph);
        tarjan.execute();
        steps = tarjan.getSteps();
    }
    if(m_algorithm == "Kahn") {
        Kahn kahn(m_graph);
        kahn.execute();
        steps = kahn.getSteps();
    }

    emit stepsReady(steps); // signal da su koraci spremni, u isto vreme ih i saljemo
}
