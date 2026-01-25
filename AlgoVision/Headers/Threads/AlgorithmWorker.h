#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>

#include "AlgorithmStep.h"
#include "AlgorithmController.h"
#include "AlgorithmStepApplier.h"
#include "Graph.h"
#include "AStar.h"
#include "BFS.h"
#include "DFS.h"
#include "Tarjan.h"
#include "Kahn.h"
#include "BellmanFord.h"
#include "FloydWarshall.h"
#include "Prim.h"
#include "Dijkstra.h"
#include "UnweightedDirectedGraph.h"
#include "UnweightedUndirectedGraph.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"

class AlgorithmWorker : public QThread {
    Q_OBJECT
public:
    AlgorithmWorker(const QString&, std::shared_ptr<Graph>, unsigned = 0, unsigned = 0);

signals:
    void stepsReady(const std::vector<AlgorithmStep>&);

protected:
    void run() override;

private:
    QString m_algorithm;
    std::shared_ptr<Graph> m_graph;
    unsigned m_start;
    unsigned m_end;
};

#endif // WORKER_H


