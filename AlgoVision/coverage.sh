#!/bin/bash

set -e  # prekida skriptu ako neka komanda padne

echo "--- building project with coverage flags ---"

mkdir -p build-coverage
cd build-coverage

cmake .. -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS_ONLY=ON -DCMAKE_CXX_FLAGS="--coverage"
cmake --build . --target AlgoVisionTests

echo "--- running tests ---"
./AlgoVisionTests

echo "--- generating coverage report ---"

lcov --capture \
  --directory CMakeFiles/AlgoVisionTests.dir/Source/Graph \
  --output-file coverage_graph.info

lcov --capture \
  --directory CMakeFiles/AlgoVisionTests.dir/Source/Algorithm \
  --output-file coverage_algorithm.info

lcov -a coverage_graph.info -a coverage_algorithm.info -o coverage_total.info

genhtml coverage_total.info --output-directory coverage-report

# google-chrome coverage-report/index.html
