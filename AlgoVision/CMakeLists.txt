cmake_minimum_required(VERSION 3.16)

project(AlgoVision VERSION 0.1 LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.2
)

FetchContent_MakeAvailable(Catch2)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        Source/main.cpp
        Forms/MainWindow.ui Headers/MainWindow.h Source/MainWindow.cpp
        
        Headers/Graph/Graph.h Source/Graph/Graph.cpp
        Headers/Graph/Node.h Source/Graph/Node.cpp
        Headers/Graph/Edge.h Source/Graph/Edge.cpp
        Headers/Graph/UnweightedUndirectedGraph.h Source/Graph/UnweightedUndirectedGraph.cpp
        Headers/Graph/UnweightedDirectedGraph.h Source/Graph/UnweightedDirectedGraph.cpp
        Headers/Graph/WeightedUndirectedGraph.h Source/Graph/WeightedUndirectedGraph.cpp
        Headers/Graph/WeightedDirectedGraph.h Source/Graph/WeightedDirectedGraph.cpp

        Headers/Algorithm/Algorithm.h Source/Algorithm/Algorithm.cpp
        Headers/Algorithm/DFS.h Source/Algorithm/DFS.cpp
        Headers/Algorithm/BFS.h Source/Algorithm/BFS.cpp
        Headers/Algorithm/Tarjan.h Source/Algorithm/Tarjan.cpp
        Headers/Algorithm/Dijkstra.h Source/Algorithm/Dijkstra.cpp
        Headers/Algorithm/BellmanFord.h Source/Algorithm/BellmanFord.cpp
        Headers/Algorithm/FloydWarshall.h Source/Algorithm/FloydWarshall.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PROJECT_SOURCES})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(AlgoVision
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET AlgoVision APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(AlgoVision SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(AlgoVision
            ${PROJECT_SOURCES}
        )
    endif()
endif()

set_target_properties(AlgoVision PROPERTIES
    AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/Forms"
)

target_link_libraries(AlgoVision PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_include_directories(AlgoVision PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Headers
    ${CMAKE_CURRENT_SOURCE_DIR}/Headers/Algorithm
    ${CMAKE_CURRENT_SOURCE_DIR}/Headers/Graph
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.AlgoVision)
endif()
set_target_properties(AlgoVision PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS AlgoVision
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(AlgoVision)
endif()

add_executable(AlgoVisionTests
    Source/Graph/Graph.cpp
    Source/Graph/Node.cpp
    Source/Graph/Edge.cpp
    Source/Graph/UnweightedUndirectedGraph.cpp
    Source/Graph/UnweightedDirectedGraph.cpp
    Source/Graph/WeightedUndirectedGraph.cpp
    Source/Graph/WeightedDirectedGraph.cpp

    Source/Algorithm/Algorithm.cpp
    Source/Algorithm/DFS.cpp
    Source/Algorithm/BFS.cpp
    Source/Algorithm/Tarjan.cpp
    Source/Algorithm/Dijkstra.cpp
    Source/Algorithm/BellmanFord.cpp
    Source/Algorithm/FloydWarshall.cpp

    Tests/Algorithm/dfsTest.cpp
    Tests/Algorithm/bfsTest.cpp
    Tests/Algorithm/tarjanTest.cpp
    Tests/Algorithm/dijkstraTest.cpp
    Tests/Algorithm/bellmanFordTest.cpp
    Tests/Algorithm/floydWarshallTest.cpp
)

target_link_libraries(AlgoVisionTests
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    Catch2::Catch2WithMain
)

target_include_directories(AlgoVisionTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Headers
    ${CMAKE_CURRENT_SOURCE_DIR}/Headers/Algorithm
    ${CMAKE_CURRENT_SOURCE_DIR}/Headers/Graph
)
