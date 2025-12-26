#include "MainWindow.h"
#include "Graph.h"
#include "Node.h"
#include "Edge.h"

#include <QApplication>
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "test" << std::endl;
    QApplication a(argc, argv);
    MainWindow   w;
    w.show();
    return a.exec();
}
