#include <iostream>
#include <memory>

#include "WeightedDirectedGraph.h"
#include "UnweightedDirectedGraph.h"
#include "Tarjan.h"

void testUDG() {
    auto udg = std::make_shared<UnweightedDirectedGraph>();
    for(unsigned i = 1; i <= 5; ++i) {
        udg->addNode(i);
    }

    udg->addEdge(1, 2);
    udg->addEdge(2, 3);
    udg->addEdge(3, 1);
    udg->addEdge(3, 4);
    udg->addEdge(4, 5);

    std::cout << "UDG" << std::endl;
    Tarjan tarjan(udg);
    try {
        tarjan.execute();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;
}

void testWDG() {
    auto wdg = std::make_shared<WeightedDirectedGraph>();
    for(unsigned i = 1; i <= 6; ++i) {
        wdg->addNode(i);
    }

    wdg->addEdge(1, 2, 1);
    wdg->addEdge(2, 3, 2);
    wdg->addEdge(3, 1, 3);
    wdg->addEdge(3, 4, 1);
    wdg->addEdge(4, 5, 2);
    wdg->addEdge(5, 6, 1);
    wdg->addEdge(6, 4, 3);

    std::cout << "WDG" << std::endl;
    Tarjan tarjan(wdg);
    try {
        tarjan.execute();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;
}

int main() {
    testUDG();
    testWDG();
    return 0;
}
