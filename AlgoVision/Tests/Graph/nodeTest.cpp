#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "Node.h"

TEST_CASE("Observer is called on state change", "[Node]") {
    Node n(1, 0.0, 0.0);

    int callCount = 0;
    NodeState lastState = NodeState::Default;

    n.addObserver([&](Node& node){
        callCount++;
        lastState = node.getState();
    });

    n.setState(NodeState::Active);
    REQUIRE(callCount == 1);
    REQUIRE(lastState == NodeState::Active);

    n.setState(NodeState::Visited);
    REQUIRE(callCount == 2);
    REQUIRE(lastState == NodeState::Visited);
}

TEST_CASE("Observer is not called if state does not change", "[Node]") {
    Node n(1, 0.0, 0.0);

    int callCount = 0;
    n.addObserver([&](Node& node){
        callCount++;
    });

    n.setState(NodeState::Default); // isto kao inicijalno
    REQUIRE(callCount == 0);        // ne bi trebao da se pozove
}

TEST_CASE("Observer is not called after removal", "[Node]") {
    Node n(1, 0.0, 0.0);

    int callCount = 0;
    unsigned obsId = n.addObserver([&](Node& node){
        callCount++;
    });

    n.removeObserver(obsId);
    n.setState(NodeState::Active);
    REQUIRE(callCount == 0); // observer je uklonjen
}
