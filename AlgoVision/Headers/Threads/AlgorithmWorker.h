#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>

#include "AStar.h"
#include "AlgorithmController.h"
#include "AlgorithmError.h"
#include "AlgorithmStep.h"
#include "AlgorithmStepApplier.h"
#include "BFS.h"
#include "BellmanFord.h"
#include "DFS.h"
#include "Dijkstra.h"
#include "FloydWarshall.h"
#include "Graph.h"
#include "Kahn.h"
#include "Prim.h"
#include "Tarjan.h"
#include "UnweightedDirectedGraph.h"
#include "UnweightedUndirectedGraph.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"

class AlgorithmWorker : public QThread {
    Q_OBJECT
public:
    AlgorithmWorker(const QString&, const std::shared_ptr<Graph>, unsigned = 0, unsigned = 0);

signals:
    void stepsReady(const std::vector<AlgorithmStep>&);
    void resultReady(const QString&);
    void algorithmErrorOccurred(const AlgorithmError&);

protected:
    void run() override;

private:
    QString                m_algorithm;
    std::shared_ptr<Graph> m_graph;
    unsigned               m_start;
    unsigned               m_end;
};

#endif // WORKER_H
