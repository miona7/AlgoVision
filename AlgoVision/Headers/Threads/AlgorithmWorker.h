#ifndef ALGORITHMWORKER_H
#define ALGORITHMWORKER_H

#include <QObject>
#include <QString>
#include <QThread>

#include <memory>

#include "AStar.h"
#include "BFS.h"
#include "BellmanFord.h"
#include "DFS.h"
#include "Dijkstra.h"
#include "FloydWarshall.h"
#include "Graph.h"
#include "Kahn.h"
#include "Prim.h"
#include "Tarjan.h"
#include "AlgorithmStep.h"
#include "AlgorithmError.h"
#include "AlgorithmStepApplier.h"
#include "AlgorithmController.h"
#include "UnweightedDirectedGraph.h"
#include "UnweightedUndirectedGraph.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"

/**
 * @class AlgorithmWorker
 * @brief Executes graph algorithms in a separate thread and generates visualization steps.
 *
 * This class inherits from QThread to run the algorithm asynchronously.
 * During execution, it emits signals to communicate:
 *  - when the steps are ready for visualization,
 *  - when the algorithm result is ready,
 *  - if an error occurs during execution.
 *
 */

class AlgorithmWorker : public QThread {
    Q_OBJECT
public:
    AlgorithmWorker(const QString&, const std::shared_ptr<Graph>, unsigned = 0, unsigned = 0,
                    QObject* = nullptr);

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

#endif // ALGORITHMWORKER_H
