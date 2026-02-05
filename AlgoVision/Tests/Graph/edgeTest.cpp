#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "Edge.h"

TEST_CASE("Observer is called on edge state change", "[Edge]") {
    Edge e(1, 0, 1, 5);

    int       callCount = 0;
    EdgeState lastState = EdgeState::Default;

    e.addObserver([&](Edge& edge) {
        callCount++;
        lastState = edge.getState();
    });

    e.setState(EdgeState::Examined);

    REQUIRE(callCount == 1);
    REQUIRE(lastState == EdgeState::Examined);

    e.setState(EdgeState::Relaxed);

    REQUIRE(callCount == 2);
    REQUIRE(lastState == EdgeState::Relaxed);
}

TEST_CASE("Observer is not called if edge state does not change", "[Edge]") {
    Edge e(1, 0, 1, 5);

    int callCount = 0;
    e.addObserver([&](Edge& edge) { callCount++; });

    e.setState(EdgeState::Default);

    REQUIRE(callCount == 0);
}

TEST_CASE("Observer is not called after removal", "[Edge]") {
    Edge e(1, 0, 1, 5);

    int      callCount = 0;
    unsigned obsId     = e.addObserver([&](Edge& edge) { callCount++; });

    e.removeObserver(obsId);
    e.setState(EdgeState::Selected);

    REQUIRE(callCount == 0);
}
